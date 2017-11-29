/*
Copyright (c) 2017 InversePalindrome
Nihil - HealthComponent.cpp
InversePalindrome.com
*/


#include "HealthComponent.hpp"


HealthComponent::HealthComponent(std::int32_t hitpoints) :
	Component("Health"),
	hitpoints(hitpoints)
{
}

std::ostream& operator<<(std::ostream& os, const HealthComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.hitpoints;

	return os;
}

std::int32_t HealthComponent::getHitpoints() const
{
	return this->hitpoints;
}

void HealthComponent::setHitpoints(std::int32_t hitpoints)
{
	this->hitpoints = hitpoints;
}