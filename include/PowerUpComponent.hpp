/*
Copyright (c) 2017 InversePalindrome
Nihil - PowerUpComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "PickupComponent.hpp"


class PowerUpComponent : public PickupComponent
{
public:
	PowerUpComponent(Item item, SoundBuffersID soundID, float effectTime, float effectBoost);

	float getEffectTime() const;
	float getEffectBoost() const;

	void setEffectTime(float effectTime);
	void setEffectBoost(float effectBoost);

private:
	float effectBoost;
	float effectTime;
};