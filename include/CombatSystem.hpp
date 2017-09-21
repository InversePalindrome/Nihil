/*
(c) 2017 InversePalindrome
Nihil - CombatSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "ComponentParser.hpp"


class CombatSystem : public System
{
public:
	CombatSystem(Entities& entities, Events& events, ComponentParser& componentParser);

	virtual void update(float deltaTime) override;

private:
	ComponentParser& componentParser;

	void handleCombat(Entity attacker, Entity victim);

	void shootProjectile(Entity shooter, const std::string& projectileID);
	void addReloadTimer(Entity entity, RangeAttackComponent& rangeAttack);
};