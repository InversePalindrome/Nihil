/*
Copyright (c) 2017 InversePalindrome
Nihil - AttackComponent.cpp
InversePalindrome.com
*/


#include "MeleeAttackComponent.hpp"


MeleeAttackComponent::MeleeAttackComponent(std::int32_t damagePoints) :
	Component("MeleeA"),
	damagePoints(damagePoints),
	knockback(0.f)
{
}

MeleeAttackComponent::MeleeAttackComponent(std::int32_t damagePoints, float knockback) :
	Component("MeleeB"),
	damagePoints(damagePoints),
	knockback(knockback)
{
}

std::ostream& operator<<(std::ostream& os, const MeleeAttackComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.damagePoints << ' ' << component.knockback;

	return os;
}

std::int32_t MeleeAttackComponent::getDamagePoints() const
{
	return this->damagePoints;
}

float MeleeAttackComponent::getKnockback() const
{
	return this->knockback;
}

void MeleeAttackComponent::setDamagePoints(std::int32_t damagePoints)
{
	this->damagePoints = damagePoints;
}

void MeleeAttackComponent::setKnockback(float knockback)
{
	this->knockback = knockback;
}