/*
Copyright (c) 2017 InversePalindrome
Nihil - RangeAttackComponent.cpp
InversePalindrome.com
*/


#include "RangeAttackComponent.hpp"


RangeAttackComponent::RangeAttackComponent(std::size_t damagePoints, float reloadTime, float attackRange) :
	Component("Range"),
	damagePoints(damagePoints),
	reloadTime(reloadTime),
	attackRange(attackRange)
{
}

std::ostream& operator<<(std::ostream& os, const RangeAttackComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.damagePoints << ' ' << component.reloadTime << ' ' << component.attackRange;

	return os;
}

std::size_t RangeAttackComponent::getDamagePoints() const
{
	return this->damagePoints;
}

float RangeAttackComponent::getReloadTime() const
{
	return this->reloadTime;
}

float RangeAttackComponent::getAttackRange() const
{
	return this->attackRange;
}

void RangeAttackComponent::setDamagePoints(std::size_t damagePoints)
{
	this->damagePoints = damagePoints;
}

void RangeAttackComponent::setReloadTime(float reloadTime)
{
	this->reloadTime = reloadTime;
}

void RangeAttackComponent::setAttackRange(float attackRange)
{
	this->attackRange = attackRange;
}