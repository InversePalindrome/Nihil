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
	RangeAttackComponent(const std::string& projectileID, float reloadTime);
	RangeAttackComponent(const std::string& projectileID, float reloadTime, float attackRange);

	std::string getProjectileID() const;
	float getReloadTime() const;
	float getAttackRange() const;

	void setProjectileID(const std::string& projectileID);
	void setReloadTime(float reloadTime);
	void setAttackRange(float attackRange);

private:
	std::string projectileID;
	float reloadTime;
	float attackRange;
};

std::ostream& operator<<(std::ostream& os, const RangeAttackComponent& component);