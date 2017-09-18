/*
Copyright (c) 2017 InversePalindrome
Nihil - RangeAttackComponent.cpp
InversePalindrome.com
*/


#include "RangeAttackComponent.hpp"


RangeAttackComponent::RangeAttackComponent(std::size_t damagePoints, float attackRange, float reloadTime) :
	Component("Range"),
	damagePoints(damagePoints),
	attackRange(attackRange),
	reloadTime(reloadTime)
{
}

std::ostream& operator<<(std::ostream& os, const RangeAttackComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.damagePoints << ' ' << component.attackRange;

	return os;
}

std::size_t RangeAttackComponent::getDamagePoints() const
{
	return this->damagePoints;
}

float RangeAttackComponent::getAttackRange() const
{
	return this->attackRange;
}

float RangeAttackComponent::getReloadTime() const
{
	return this->reloadTime;
}

void RangeAttackComponent::setDamagePoints(std::size_t damagePoints)
{
	this->damagePoints = damagePoints;
}

void RangeAttackComponent::setAttackRange(float attackRange)
{
	this->attackRange = attackRange;
}

void RangeAttackComponent::setReloadTime(float reloadTime)
{
	this->reloadTime = reloadTime;
}