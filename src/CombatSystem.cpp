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
    events.subscribe<entityplus::component_added<Entity, HealthComponent>>([&events](const auto & event)
        {
            if (event.entity.has_component<ControllableComponent>())
            {
                events.broadcast(DisplayHealthBar{ event.component });
            }
        });

    events.subscribe<entityplus::component_added<Entity, ControllableComponent>>([this](const auto & event)
        {
            targetEntity = event.entity;
        });

    events.subscribe<entityplus::component_added<Entity, RangeAttackComponent>>([this](const auto & event) { addReloadTimer(event.entity); });
    events.subscribe<CombatOcurred>([this](const auto & event) { handleCombat(event.attacker, event.victim); });
    events.subscribe<ShootProjectile>([this](const auto & event) { shootProjectile(event.shooter, event.projectileID); });
    events.subscribe<ActivateBomb>([this](const auto & event) { addExplosion(event.bomb); });
    events.subscribe<ApplyKnockback>([this](const auto & event) { applyKnockback(event.attacker, event.victim); });
    events.subscribe<ApplyBlastImpact>([this](const auto & event) { applyBlastImpact(event.explosion, event.victim); });
}

void CombatSystem::update(float deltaTime)
{
    this->callbacks.update();
    this->callbacks.clearCallbacks();
}

void CombatSystem::handleCombat(Entity attacker, Entity victim)
{
    std::int32_t damagePoints = 0;

    if (attacker.has_component<MeleeAttackComponent>())
    {
        damagePoints = attacker.get_component<MeleeAttackComponent>().getDamagePoints();
    }
    else if (attacker.has_component<BulletComponent>())
    {
        damagePoints = attacker.get_component<BulletComponent>().getDamagePoints();
    }
    else if (attacker.has_component<BombComponent>())
    {
        damagePoints = attacker.get_component<BombComponent>().getDamagePoints();
    }

    if (victim.has_component<HealthComponent>())
    {
        auto& health = victim.get_component<HealthComponent>();

        if (health.getHitpoints() > 0)
        {
            health.setHitpoints(health.getHitpoints() - damagePoints);
        }
        if (victim.has_component<ControllableComponent>())
        {
            this->events.broadcast(DisplayHealthBar{ health });
        }
        if (health.getHitpoints() <= 0)
        {
            if (victim.has_component<DropComponent>())
            {
                this->events.broadcast(DroppedItem{ victim });
            }

            this->events.broadcast(DestroyEntity{ victim });
        }
    }
}

void CombatSystem::handleExplosion(Entity bomb, Entity explosion)
{
    const float explosionTime = 0.5f;

    this->callbacks.addCallbackTimer([this, explosion, bomb]() mutable
        {
            this->events.broadcast(DestroyEntity{ bomb });
            this->events.broadcast(DestroyEntity{ explosion });
        }, explosionTime);
}

void CombatSystem::shootProjectile(Entity shooter, const std::string & projectileID)
{
    if (this->canShoot(shooter))
    {
        auto projectileEntity = this->componentParser.parseEntity(-1, projectileID + ".txt");

        this->events.broadcast(ManageCollision{ shooter, projectileEntity, false });

        if (shooter.has_component<ParentComponent>() && projectileEntity.has_component<ChildComponent>())
        {
            this->events.broadcast(CreateTransform{ projectileEntity, shooter, this->getProjectileOffset(shooter) });

            if (shooter.has_tag<Turret>())
            {
                this->events.broadcast(PlayAnimation{ shooter, { EntityState::Attacking, Direction::Right }, false });
            }
        }
        if (shooter.has_component<PhysicsComponent>() && projectileEntity.has_component<PhysicsComponent>() && projectileEntity.has_component<SpriteComponent>())
        {
            const auto& shooterPhysics = shooter.get_component<PhysicsComponent>();
            auto& projectilePhysics = projectileEntity.get_component<PhysicsComponent>();
            auto& projectileSprite = projectileEntity.get_component<SpriteComponent>();

            if (projectileEntity.has_component<BulletComponent>())
            {
                this->shootBullet(shooterPhysics, projectileEntity.get_component<BulletComponent>(), projectilePhysics, projectileSprite);
            }
            else if (projectileEntity.has_component<BombComponent>())
            {
                this->shootBomb(shooterPhysics, projectilePhysics);
            }
        }
    }
}

void CombatSystem::shootBullet(const PhysicsComponent & shooterPhysics, BulletComponent & bulletComponent, PhysicsComponent & projectilePhysics, SpriteComponent & projectileSprite)
{
    switch (shooterPhysics.getDirection())
    {
    case Direction::Left:
        projectilePhysics.applyForce({ -bulletComponent.getForce(), 0.f });
        projectileSprite.setRotation(180.f);
        break;
    case Direction::Right:
        projectilePhysics.applyForce({ bulletComponent.getForce(), 0.f });
        projectileSprite.setRotation(0.f);
        break;
    }

    this->events.broadcast(PlaySound{ bulletComponent.getSoundID(), false });
}

void CombatSystem::shootBomb(const PhysicsComponent & shooterPhysics, PhysicsComponent & projectilePhysics)
{
    if (this->targetEntity.sync() && this->targetEntity.has_component<PhysicsComponent>())
    {
        const auto& targetPhysics = this->targetEntity.get_component<PhysicsComponent>();

        const auto xDistance = projectilePhysics.getPosition().x - targetPhysics.getPosition().x;
        const auto angle = 45.f;

        const auto velocity = Utility::calculateRequiredVelocity(9.8f, std::abs(xDistance), angle, std::abs(projectilePhysics.getPosition().y));

        if (!std::isnan(velocity))
        {
            if (shooterPhysics.getPosition().x > targetPhysics.getPosition().x)
            {
                projectilePhysics.setVelocity({ -velocity * std::cos(angle), velocity * std::sin(angle) });
            }
            else
            {
                projectilePhysics.setVelocity({ velocity * std::cos(angle), velocity * std::sin(angle) });
            }
        }
    }
}

void CombatSystem::addReloadTimer(Entity entity)
{
    if (entity.has_component<TimerComponent>())
    {
        auto& timer = entity.get_component<TimerComponent>();

        timer.addTimer("Reload", entity.get_component<RangeAttackComponent>().getReloadTime());
        timer.restartTimer("Reload");
    }
}

void CombatSystem::addExplosion(Entity bomb)
{
    if (bomb.has_component<BombComponent>() && !bomb.get_component<BombComponent>().hasActivated())
    {
        bomb.get_component<BombComponent>().setActivationStatus(true);

        this->callbacks.addCallbackTimer(
            [this, bomb]() mutable
            {
                if (bomb.sync())
                {
                    auto& bombComponent = bomb.get_component<BombComponent>();

                    auto explosion = this->componentParser.parseEntity(-1, bombComponent.getExplosionID() + ".txt");

                    this->events.broadcast(CreateTransform{ explosion, bomb, {0.f, 0.f} });
                    this->events.broadcast(PlaySound{ bombComponent.getSoundID(), false });

                    this->handleExplosion(bomb, explosion);
                }
            }, bomb.get_component<BombComponent>().getExplosionTime());
    }
}

void CombatSystem::applyKnockback(Entity attacker, Entity victim)
{
    if (attacker.has_component<PhysicsComponent>() && victim.has_component<PhysicsComponent>())
    {
        const auto& attackerPhysics = attacker.get_component<PhysicsComponent>();
        auto& victimPhysics = victim.get_component<PhysicsComponent>();

        float knockback = 0.f;

        if (attacker.has_component<MeleeAttackComponent>())
        {
            knockback = attacker.get_component<MeleeAttackComponent>().getKnockback();

            switch (attackerPhysics.getDirection())
            {
            case Direction::Left:
                victimPhysics.applyImpulse({ -knockback, 0.f });
                break;
            case Direction::Right:
                victimPhysics.applyImpulse({ knockback, 0.f });
                break;
            }
        }
        else if (attacker.has_component<BombComponent>())
        {
            knockback = attacker.get_component<BombComponent>().getExplosionKnockback();

            const auto& blastDistance = victimPhysics.getPosition() - attackerPhysics.getPosition();

            victimPhysics.applyImpulse({ Utility::sign(blastDistance.x) * knockback, 0.f });

            this->handleCombat(attacker, victim);
        }
    }
}

void CombatSystem::applyBlastImpact(Entity explosion, Entity victim)
{
    if (explosion.has_component<ChildComponent>())
    {
        auto childID = explosion.get_component<ChildComponent>().getEntityID();

        this->entities.for_each<BombComponent, ParentComponent>([this, victim, childID](auto entity, const auto & bomb, const auto & parent, entityplus::control_block_t & control)
            {
                if (parent.getChildID() == childID)
                {
                    this->applyKnockback(entity, victim);
                    control.breakout = true;
                }
            });
    }
}

sf::Vector2f CombatSystem::getProjectileOffset(Entity entity)
{
    if (entity.has_component<PhysicsComponent>())
    {
        auto& physics = entity.get_component<PhysicsComponent>();

        auto direction = physics.getDirection();
        const auto& size = physics.getBodySize();

        if (direction == Direction::Left)
        {
            return { UnitConverter::metersToPixels(-size.x), 0.f };
        }
        else if (direction == Direction::Right)
        {
            return { UnitConverter::metersToPixels(size.x), 0.f };
        }
        else if (direction == Direction::Up)
        {
            return { 0.f, UnitConverter::metersToPixels(size.y) };
        }
        else if (direction == Direction::Down)
        {
            return { 0.f, UnitConverter::metersToPixels(-size.y) };
        }
    }

    return {};
}

bool CombatSystem::canShoot(Entity shooter)
{
    if (this->targetEntity.sync() && shooter.has_component<PhysicsComponent>() && this->targetEntity.has_component<PhysicsComponent>() && shooter.has_tag<Turret>())
    {
        const auto& shooterPhysics = shooter.get_component<PhysicsComponent>();
        const auto& targetPhysics = this->targetEntity.get_component<PhysicsComponent>();

        return std::abs(shooterPhysics.getPosition().x - targetPhysics.getPosition().x) >= shooterPhysics.getBodySize().x;
    }

    return true;
}