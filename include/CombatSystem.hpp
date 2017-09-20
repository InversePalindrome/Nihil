/*
(c) 2017 InversePalindrome
Nihil - CombatSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"


class CombatSystem : public System
{
public:
	CombatSystem(Entities& entities, Events& events);

	virtual void update(float deltaTime) override;

private:
	void handleCombat(Entity attacker, Entity victim);

	void shootProjectile();
	void addReloadTimer(Entity entity, RangeAttackComponent& rangeAttack);
};