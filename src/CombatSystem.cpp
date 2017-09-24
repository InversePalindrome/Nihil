/*
Copyright (c) 2017 InversePalindrome
Nihil - CombatSystem.cpp
InversePalindrome.com
*/


#include "CombatSystem.hpp"


CombatSystem::CombatSystem(Entities& entities, Events& events, ComponentParser& componentParser) :
	System(entities, events),
	componentParser(componentParser)
{
	events.subscribe<entityplus::component_added<Entity, HealthComponent>>([&events](const auto& event)
	{
		if (event.entity.has_component<ControllableComponent>())
		{
			events.broadcast(DisplayHealthBar{ event.entity });
		}
	});

	events.subscribe<entityplus::component_added<Entity, RangeAttackComponent>>([this](const auto& event) { addReloadTimer(event.entity, event.component); });
	events.subscribe<CombatOcurred>([this](const auto& event) { handleCombat(event.attacker, event.victim); });
	events.subscribe<ShootProjectile>([this](const auto& event) { shootProjectile(event.shooter, event.projectileID); });
}

void CombatSystem::update(float deltaTime)
{
	
}

void CombatSystem::handleCombat(Entity attacker, Entity victim)
{
	std::size_t damagePoints = 0u;

	if (attacker.has_component<MeleeAttackComponent>())
	{
		damagePoints = attacker.get_component<MeleeAttackComponent>().getDamagePoints();
	}
	else if (attacker.has_component<ProjectileComponent>())
	{
		damagePoints = attacker.get_component<ProjectileComponent>().getDamagePoints();
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
			this->events.broadcast(DisplayHealthBar{ victim });
		}
		if (health.getHitpoints() == 0u)
		{
			this->events.broadcast(DestroyEntity{ victim });
		}
	}
}

void CombatSystem::shootProjectile(Entity shooter, const std::string& projectileID)
{
	auto& projectileEntity = this->componentParser.parseComponents("Resources/Files/" + projectileID + ".txt");
	
	this->componentParser.setComponentsID(projectileEntity, -1);
	
	if (shooter.has_component<ParentComponent>() && projectileEntity.has_component<ChildComponent>())
	{
		this->events.broadcast(CreateTransform{ projectileEntity, projectileEntity.get_component<ChildComponent>(), shooter.get_component<ParentComponent>() });
	}
	if (shooter.has_component<PhysicsComponent>() && projectileEntity.has_component<ProjectileComponent>() && projectileEntity.has_component<PhysicsComponent>()
		&& projectileEntity.has_component<SpriteComponent>())
	{
		const auto& shooterPhysics = shooter.get_component<PhysicsComponent>();
		
		auto& projectileComponent = projectileEntity.get_component<ProjectileComponent>();
		auto& projectilePhysics = projectileEntity.get_component<PhysicsComponent>();
		auto& projectileSprite = projectileEntity.get_component<SpriteComponent>();

		switch (shooterPhysics.getDirection())
		{
		case Direction::Left:
			projectilePhysics.setPosition(b2Vec2(projectilePhysics.getPosition().x - projectilePhysics.getBodySize().x - 0.1f - shooterPhysics.getBodySize().x, projectilePhysics.getPosition().y));
			projectilePhysics.applyForce(b2Vec2(-projectileComponent.getSpeed(), 0.f));
			projectileSprite.setRotation(180.f);
			break;
		case Direction::Right:
			projectilePhysics.setPosition(b2Vec2(projectilePhysics.getPosition().x + projectilePhysics.getBodySize().x + 0.1f + shooterPhysics.getBodySize().x, projectilePhysics.getPosition().y));
			projectilePhysics.applyForce(b2Vec2(projectileComponent.getSpeed(), 0.f));
			projectileSprite.setRotation(0.f);
			break;
		}

		this->events.broadcast(EmitSound{ projectileComponent.getSoundID(), false });
	}
}

void CombatSystem::addReloadTimer(Entity entity, RangeAttackComponent& rangeAttack)
{
	if (entity.has_component<TimerComponent>())
	{
		auto& timer = entity.get_component<TimerComponent>();

		timer.addTimer("Reload", rangeAttack.getReloadTime());
		timer.restartTimer("Reload");
	}
}