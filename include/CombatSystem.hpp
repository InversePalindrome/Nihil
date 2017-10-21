/*
(c) 2017 InversePalindrome
Nihil - CombatSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "Callbacks.hpp"
#include "ComponentParser.hpp"

#include <Thor/Time/CallbackTimer.hpp>


class CombatSystem : public System
{
public:
	CombatSystem(Entities& entities, Events& events, ComponentParser& componentParser);

	virtual void update(float deltaTime) override;

private:
	ComponentParser& componentParser;
	Callbacks callbacks;

	void handleCombat(Entity attacker, Entity victim);
	void handleExplosion(Entity bomb, Entity explosion);

	void shootProjectile(Entity shooter, const std::string& projectileID, const sf::Vector2f& targetPosition);
	void shootBullet(const PhysicsComponent& shooterPhysics, BulletComponent& bulletComponent, PhysicsComponent& physicsComponent, SpriteComponent& spriteComponent);
	void shootBomb(const PhysicsComponent& shooterPhysics, BombComponent& bombComponent, PhysicsComponent& physicsComponent, SpriteComponent& spriteComponent,
		const sf::Vector2f& targetPosition);

	void addReloadTimer(Entity entity);
	void addExplosion(Entity bomb);

	void applyKnockback(Entity attacker, Entity victim);
	void applyBlastImpact(Entity explosion, Entity victim);
};