/*
Copyright (c) 2017 InversePalindrome
Nihil - HealthComponent.cpp
InversePalindrome.com
*/


#include "HealthComponent.hpp"


HealthComponent::HealthComponent(std::size_t hitpoints) :
	hitpoints(hitpoints)
{
}

std::size_t HealthComponent::getHitpoints() const
{
	return this->hitpoints;
}

void HealthComponent::setHitpoints(std::size_t hitpoints)
{
	this->hitpoints = hitpoints;
}