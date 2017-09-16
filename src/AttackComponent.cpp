/*
Copyright (c) 2017 InversePalindrome
Nihil - AttackComponent.cpp
InversePalindrome.com
*/


#include "AttackComponent.hpp"


AttackComponent::AttackComponent(std::size_t damagePoints) :
	Component("Attack"),
	damagePoints(damagePoints)
{
}

std::ostream& operator<<(std::ostream& os, const AttackComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.damagePoints;

	return os;
}

std::size_t AttackComponent::getDamagePoints() const
{
	return this->damagePoints;
}

void AttackComponent::setDamagePoints(std::size_t damagePoints)
{
	this->damagePoints = damagePoints;
}
