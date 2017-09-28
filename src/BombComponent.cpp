/*
Copyright (c) 2017 InversePalindrome
Nihil - BombComponent.cpp
InversePalindrome.com
*/


#include "BombComponent.hpp"


BombComponent::BombComponent(std::size_t damagePoints, SoundBuffersID soundID, float explosionTime, const std::string& explosionID) :
	Component("Bomb"),
	Projectile(damagePoints, soundID),
	explosionTime(explosionTime),
	explosionID(explosionID),
	activationStatus(false)
{
}

std::ostream& operator<<(std::ostream& os, const BombComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.getDamagePoints() << ' ' << static_cast<std::size_t>(component.getSoundID())
		<< ' ' << component.explosionTime;

	return os;
}

float BombComponent::getExplosionTime() const
{
	return this->explosionTime;
}

std::string BombComponent::getExplosionID() const
{
	return this->explosionID;
}

void BombComponent::setExplosionTime(float explsionTime)
{
	this->explosionTime = explosionTime;
}

void BombComponent::setExplosionID(const std::string& explosionID)
{
	this->explosionID = explosionID;
}

void BombComponent::setActivationStatus(bool explosionStatus)
{
	this->activationStatus = explosionStatus;
}

bool BombComponent::hasActivated() const
{
	return this->activationStatus;
}