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
	PowerUpComponent(Item item, SoundBuffersID soundID);
};