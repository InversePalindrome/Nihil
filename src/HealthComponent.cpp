/*
Copyright (c) 2017 InversePalindrome
Nihil - HealthComponent.cpp
InversePalindrome.com
*/


#include "HealthComponent.hpp"


HealthComponent::HealthComponent(std::size_t hitpoints) :
	Component(ComponentID::Health),
	hitpoints(hitpoints)
{
}

std::ostream& HealthComponent::operator<<(std::ostream& os)
{
	os << this->hitpoints;

	return os;
}

std::size_t HealthComponent::getHitpoints() const
{
	return this->hitpoints;
}

void HealthComponent::setHitpoints(std::size_t hitpoints)
{
	this->hitpoints = hitpoints;
}