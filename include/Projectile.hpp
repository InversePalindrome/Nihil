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
	Projectile(std::size_t damagePoints, SoundBuffersID soundID);

	std::size_t getDamagePoints() const;
	SoundBuffersID getSoundID() const;

	void setDamagePoints(std::size_t damagePoints);
	void setSoundID(SoundBuffersID soundID);

private:
	std::size_t damagePoints;
	SoundBuffersID soundID;
};