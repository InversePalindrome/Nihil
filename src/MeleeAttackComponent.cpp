/*
Copyright (c) 2017 InversePalindrome
Nihil - AttackComponent.cpp
InversePalindrome.com
*/


#include "MeleeAttackComponent.hpp"


MeleeAttackComponent::MeleeAttackComponent(std::size_t damagePoints) :
	Component("Melee"),
	damagePoints(damagePoints)
{
}

std::ostream& operator<<(std::ostream& os, const MeleeAttackComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.damagePoints;

	return os;
}

std::size_t MeleeAttackComponent::getDamagePoints() const
{
	return this->damagePoints;
}

void MeleeAttackComponent::setDamagePoints(std::size_t damagePoints)
{
	this->damagePoints = damagePoints;
}
