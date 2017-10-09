/*
Copyright (c) 2017 InversePalindrome
Nihil - CombatSystem.cpp
InversePalindrome.com
*/


#include "CombatSystem.hpp"
#include "MathUtility.hpp"
#include "UnitConverter.hpp"
#include "FilePaths.hpp"


CombatSystem::CombatSystem(Entities& entities, Events& events, ComponentParser& componentParser) :
	System(entities, events),
	componentParser(componentParser)
{
	events.subscribe<entityplus::component_added<Entity, HealthComponent>>([&events](const auto& event)
	{
		if (event.entity.has_component<ControllableComponent>())
		{
			events.broadcast(DisplayHealthBar{ event.component });
		}
	});

	events.subscribe<entityplus::component_added<Entity, RangeAttackComponent>>([this](const auto& event) { addReloadTimer(event.entity, event.component); });
	events.subscribe<CombatOcurred>([this](const auto& event) { handleCombat(event.attacker, event.victim); });
	events.subscribe<BombExploded>([this](const auto& event) { handleExplosion(event.bomb, event.explosion); });
	events.subscribe<ShootProjectile>([this](const auto& event) { shootProjectile(event.shooter, event.projectileID, event.targetPosition ); });
	events.subscribe<ActivateBomb>([this](const auto& event) { addExplosion(event.bomb); });
	events.subscribe<ApplyKnockback>([this](const auto& event) { applyKnockback(event.attacker, event.victim); });
	events.subscribe<ApplyBlastImpact>([this](const auto& event) { applyBlastImpact(event.explosion, event.victim); });
}

void CombatSystem::update(float deltaTime)
{
	for (auto combatCallback = std::begin(this->combatCallbacks); combatCallback != std::end(this->combatCallbacks); )
	{
		combatCallback->update();

		if (combatCallback->isExpired())
		{
			combatCallback = this->combatCallbacks.erase(combatCallback);
		}
		else
		{
			++combatCallback;
		}
	}
}

void CombatSystem::handleCombat(Entity attacker, Entity victim)
{
	std::size_t damagePoints = 0u;

	if (attacker.has_component<MeleeAttackComponent>())
	{
		damagePoints = attacker.get_component<MeleeAttackComponent>().getDamagePoints();
	}
	else if ((attacker.has_component<ChildComponent>() && victim.has_component<ParentComponent>() &&
		attacker.get_component<ChildComponent>().getParentID() != victim.get_component<ParentComponent>().getChildID()) || !victim.has_component<ParentComponent>())
	{
		if (attacker.has_component<BulletComponent>())
		{
			damagePoints = attacker.get_component<BulletComponent>().getDamagePoints();
		}
		else if (attacker.has_component<BombComponent>())
		{
			damagePoints = attacker.get_component<BombComponent>().getDamagePoints();
		}
	}

	if (victim.has_component<HealthComponent>())
	{
		auto& health = victim.get_component<HealthComponent>();

		if (health.getHitpoints() > 0u)
		{
			health.setHitpoints(health.getHitpoints() - damagePoints);
		}
		if (victim.has_component<ControllableComponent>())
		{
			this->events.broadcast(DisplayHealthBar{ health });
		}
		if (health.getHitpoints() == 0u)
		{
			this->events.broadcast(DestroyEntity{ victim });
		}
	}
}

void CombatSystem::handleExplosion(Entity bomb, Entity explosion)
{
	this->combatCallbacks.push_back(thor::CallbackTimer());

    const auto explosionTime = sf::seconds(0.5f);

	this->combatCallbacks.back().restart(explosionTime);

	this->combatCallbacks.back().connect0([this, explosion, bomb]() mutable
	{
		this->events.broadcast(DestroyEntity{ bomb });
		this->events.broadcast(DestroyEntity{ explosion });
	});
}

void CombatSystem::shootProjectile(Entity shooter, const std::string& projectileID, const sf::Vector2f& targetPosition)
{
	auto projectileEntity = this->componentParser.parseComponents(-1, projectileID + ".txt");

	if (shooter.has_component<ParentComponent>() && projectileEntity.has_component<ChildComponent>())
	{
		this->events.broadcast(CreateTransform{ projectileEntity, projectileEntity.get_component<ChildComponent>(), shooter.get_component<ParentComponent>() });
	}
	if (shooter.has_component<PhysicsComponent>() && projectileEntity.has_component<PhysicsComponent>() && projectileEntity.has_component<SpriteComponent>())
	{
		
		const auto& shooterPhysics = shooter.get_component<PhysicsComponent>();
		auto& projectilePhysics = projectileEntity.get_component<PhysicsComponent>();
		auto& projectileSprite = projectileEntity.get_component<SpriteComponent>();

		if (projectileEntity.has_component<BulletComponent>())
		{
			this->shootBullet(shooter.get_component<PhysicsComponent>(), projectileEntity.get_component<BulletComponent>(), projectilePhysics, projectileSprite);
		}
		else if (projectileEntity.has_component<BombComponent>())
		{
			this->shootBomb(shooter.get_component<PhysicsComponent>(), projectileEntity.get_component<BombComponent>(), projectilePhysics, projectileSprite, targetPosition);
		}
	}
}

void CombatSystem::shootBullet(const PhysicsComponent& shooterPhysics, BulletComponent& bulletComponent, PhysicsComponent& projectilePhysics, SpriteComponent& projectileSprite)
{
	switch (shooterPhysics.getDirection())
	{
	case Direction::Left:
		projectilePhysics.setPosition(b2Vec2(projectilePhysics.getPosition().x - projectilePhysics.getBodySize().x - shooterPhysics.getBodySize().x - 0.1f, projectilePhysics.getPosition().y));
		projectilePhysics.applyForce(b2Vec2(-bulletComponent.getForce(), 0.f));
		projectileSprite.setRotation(180.f);
		break;
	case Direction::Right:
		projectilePhysics.setPosition(b2Vec2(projectilePhysics.getPosition().x + projectilePhysics.getBodySize().x + shooterPhysics.getBodySize().x + 0.1f, projectilePhysics.getPosition().y));
		projectilePhysics.applyForce(b2Vec2(bulletComponent.getForce(), 0.f));
		projectileSprite.setRotation(0.f);
		break;
	}

	this->events.broadcast(EmitSound{ bulletComponent.getSoundID(), false });
}

void CombatSystem::shootBomb(const PhysicsComponent& shooterPhysics, BombComponent& bombComponent, PhysicsComponent& projectilePhysics, SpriteComponent& spriteComponent, const sf::Vector2f& targetPosition)
{
	projectilePhysics.setPosition(b2Vec2(projectilePhysics.getPosition().x, projectilePhysics.getPosition().y + projectilePhysics.getBodySize().y + 0.1f));

	const auto xDistance = projectilePhysics.getPosition().x - UnitConverter::pixelsToMeters(targetPosition.x);
	const float angle = 45.f;

	const auto velocity = MathUtils::calculateRequiredVelocity(9.8f, std::abs(xDistance), angle, std::abs(projectilePhysics.getPosition().y));

	if (!std::isnan(velocity))
	{
		if (shooterPhysics.getPosition().x > UnitConverter::pixelsToMeters(targetPosition.x))
		{
			projectilePhysics.setVelocity(b2Vec2(-velocity * std::cos(angle), velocity * std::sin(angle)));
		}
		else
		{
			projectilePhysics.setVelocity(b2Vec2(velocity * std::cos(angle), velocity * std::sin(angle)));
		}
	}
}

void CombatSystem::addReloadTimer(Entity entity, const RangeAttackComponent& rangeAttack)
{
	if (entity.has_component<TimerComponent>())
	{
		auto& timer = entity.get_component<TimerComponent>();

		timer.addTimer("Reload", rangeAttack.getReloadTime());
		timer.restartTimer("Reload");
	}
}

void CombatSystem::addExplosion(Entity bomb)
{
	this->combatCallbacks.push_back(thor::CallbackTimer());

	if (bomb.has_component<BombComponent>() && !bomb.get_component<BombComponent>().hasActivated())
	{
		bomb.get_component<BombComponent>().setActivationStatus(true);

		this->combatCallbacks.back().restart(sf::seconds(bomb.get_component<BombComponent>().getExplosionTime()));

		this->combatCallbacks.back().connect0([this, bomb]() mutable
		{
			if (bomb.get_status() == entityplus::entity_status::OK)
			{
				auto& bombComponent = bomb.get_component<BombComponent>();

				auto explosion = this->componentParser.parseComponents(bombComponent.getExplosionID() + ".txt");

				this->componentParser.setComponentsID(explosion, -1);

				if (explosion.has_component<ChildComponent>() && bomb.has_component<ParentComponent>())
				{
					this->events.broadcast(CreateTransform{ explosion, explosion.get_component<ChildComponent>(), bomb.get_component<ParentComponent>() });
				}

				this->events.broadcast(BombExploded{ bomb, explosion });
				this->events.broadcast(EmitSound{ bombComponent.getSoundID(), false });
			}
		});
	}
}

void CombatSystem::applyBlastImpact(Entity explosion, Entity victim)
{
	if (explosion.has_component<PhysicsComponent>() && explosion.has_component<ChildComponent>() && victim.has_component<PhysicsComponent>())
	{
		auto& explosionChild = explosion.get_component<ChildComponent>();

		auto& bombs = entities.get_entities<BombComponent, ParentComponent>();

		auto foundBomb = std::find_if(std::begin(bombs), std::end(bombs), [&explosionChild](auto& bomb)
		{ 
			return explosionChild.getParentID() != -1 && explosionChild.getParentID() == bomb.get_component<ParentComponent>().getChildID(); 
		});

		if (foundBomb != std::end(bombs))
		{
			auto bombKnockback = foundBomb->get_component<BombComponent>().getExplosionKnockback();

			auto& victimPhysics = victim.get_component<PhysicsComponent>();

			auto& blastDistance = victimPhysics.getPosition() - explosion.get_component<PhysicsComponent>().getPosition();

			victimPhysics.applyImpulse(b2Vec2(MathUtils::sign(blastDistance.x) * bombKnockback, 0.f));

			this->handleCombat(*foundBomb, victim);
		}
	}
}

void CombatSystem::applyKnockback(Entity attacker, Entity victim)
{
	if (attacker.has_component<PhysicsComponent>() && attacker.has_component<MeleeAttackComponent>() &&  victim.has_component<PhysicsComponent>())
	{
		auto& victimPhysics = victim.get_component<PhysicsComponent>();

		auto attackingDirection = attacker.get_component<PhysicsComponent>().getDirection();
		auto knockback = attacker.get_component<MeleeAttackComponent>().getKnockback();

		switch (attackingDirection)
		{
		case Direction::Left:
			victimPhysics.applyImpulse(b2Vec2(-knockback, 0.f));
			break;
		case Direction::Right:
			victimPhysics.applyImpulse(b2Vec2(knockback, 0.f));
			break;
		}
	}
}