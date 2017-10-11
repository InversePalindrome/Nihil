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