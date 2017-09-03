/*
Copyright (c) 2017 InversePalindrome
Nihil - AttackComponent.cpp
InversePalindrome.com
*/


#include "AttackComponent.hpp"


AttackComponent::AttackComponent(std::size_t damagePoints) :
	Component(ComponentID::Attack),
	damagePoints(damagePoints)
{
}

std::ostream& AttackComponent::operator<<(std::ostream& os)
{
	os << this->damagePoints;

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
