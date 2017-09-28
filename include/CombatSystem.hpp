/*
(c) 2017 InversePalindrome
Nihil - CombatSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "ComponentParser.hpp"

#include <Thor/Time/CallbackTimer.hpp>

#include <list>


class CombatSystem : public System
{
public:
	CombatSystem(Entities& entities, Events& events, ComponentParser& componentParser);

	virtual void update(float deltaTime) override;

private:
	ComponentParser& componentParser;
	std::list<thor::CallbackTimer> combatCallbacks;

	void handleCombat(Entity attacker, Entity victim);
	void handleExplosion(Entity bomb, Entity explosion);

	void shootProjectile(Entity shooter, const std::string& projectileID, const sf::Vector2f& targetPosition);
	void shootBullet(const PhysicsComponent& shooterPhysics, BulletComponent& bulletComponent, PhysicsComponent& physicsComponent, SpriteComponent& spriteComponent);
	void shootBomb(const PhysicsComponent& shooterPhysics, BombComponent& bombComponent, PhysicsComponent& physicsComponent, SpriteComponent& spriteComponent,
		const sf::Vector2f& targetPosition);

	void addReloadTimer(Entity entity, RangeAttackComponent& rangeAttack);
	void addExplosion(Entity bomb);
};