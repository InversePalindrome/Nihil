/*
Copyright (c) 2017 InversePalindrome
Nihil - ProjectileComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"


class Projectile 
{
public:
	Projectile(std::int32_t damagePoints, SoundBuffersID soundID);

	std::int32_t getDamagePoints() const;
	SoundBuffersID getSoundID() const;

	void setDamagePoints(std::int32_t damagePoints);
	void setSoundID(SoundBuffersID soundID);

private:
	std::int32_t damagePoints;
	SoundBuffersID soundID;
};