/*
Copyright (c) 2017 InversePalindrome
Nihil - AttackComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <cstddef>


class MeleeAttackComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const MeleeAttackComponent& component);

public:
	MeleeAttackComponent(std::size_t damagePoints);
	MeleeAttackComponent(std::size_t damagePoints, float knockback);

	std::size_t getDamagePoints() const;
	float getKnockback() const;

	void setDamagePoints(std::size_t damagePoints);
	void setKnockback(float knockback);

private:
	std::size_t damagePoints;
	float knockback;
};

std::ostream& operator<<(std::ostream& os, const MeleeAttackComponent& component);