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
	RangeAttackComponent(std::size_t damagePoints, float reloadTime, float attackRange);

	std::size_t getDamagePoints() const;
	float getReloadTime() const;
	float getAttackRange() const;

	void setDamagePoints(std::size_t damagePoints);
	void setReloadTime(float reloadTime);
	void setAttackRange(float attackRange);

private:
	std::size_t damagePoints;
	float reloadTime;
	float attackRange;
};

std::ostream& operator<<(std::ostream& os, const RangeAttackComponent& component);