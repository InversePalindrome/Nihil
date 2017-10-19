/*
Copyright (c) 2017 InversePalindrome
Nihil - PowerUpComponent.cpp
InversePalindrome.com
*/


#include "PowerUpComponent.hpp"


PowerUpComponent::PowerUpComponent(Item item, SoundBuffersID soundID, float effectTime, float effectBoost) :
	PickupComponent(item, soundID),
	effectTime(effectTime),
	effectBoost(effectBoost)
{
	setName("PowerUp");
}

std::ostream& operator<<(std::ostream& os, const PowerUpComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << static_cast<std::size_t>(component.getItem()) 
		<< ' ' << static_cast<std::size_t>(component.getSoundID()) << ' ' << component.effectTime << ' ' << component.effectBoost;

	return os;
}

float PowerUpComponent::getEffectTime() const
{
	return this->effectTime;
}

float PowerUpComponent::getEffectBoost() const
{
	return this->effectBoost;
}

void PowerUpComponent::setEffectTime(float effectTime)
{
	this->effectTime = effectTime;
}

void PowerUpComponent::setEffectBoost(float effectBoost)
{
	this->effectBoost = effectBoost;
}