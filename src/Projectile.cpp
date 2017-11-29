/*
Copyright (c) 2017 InversePalindrome
Nihil - Projectile.cpp
InversePalindrome.com
*/


#include "Projectile.hpp"


Projectile::Projectile(std::int32_t damagePoints, SoundBuffersID soundID) :
	damagePoints(damagePoints),
	soundID(soundID)
{
}

std::int32_t Projectile::getDamagePoints() const
{
	return this->damagePoints;
}

SoundBuffersID Projectile::getSoundID() const
{
	return this->soundID;
}

void Projectile::setDamagePoints(std::int32_t damagePoints)
{
	this->damagePoints = damagePoints;
}

void Projectile::setSoundID(SoundBuffersID soundID)
{
	this->soundID = soundID;
}