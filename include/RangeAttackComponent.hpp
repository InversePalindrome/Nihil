/*
Copyright (c) 2017 InversePalindrome
Nihil - RangeAttackComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"


class RangeAttackComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const RangeAttackComponent& component);

public:
	RangeAttackComponent(std::size_t damagePoints, float attackRange, float reloadTime);

	std::size_t getDamagePoints() const;
	float getAttackRange() const;
	float getReloadTime() const;

	void setDamagePoints(std::size_t damagePoints);
	void setAttackRange(float attackRange);
	void setReloadTime(float reloadTime);

private:
	std::size_t damagePoints;
	float attackRange;
	float reloadTime;
};

std::ostream& operator<<(std::ostream& os, const RangeAttackComponent& component);