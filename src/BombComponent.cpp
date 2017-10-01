/*
Copyright (c) 2017 InversePalindrome
Nihil - BombComponent.cpp
InversePalindrome.com
*/


#include "BombComponent.hpp"


BombComponent::BombComponent(std::size_t damagePoints, SoundBuffersID soundID, float explosionTime, float explosionKnockback, const std::string& explosionID) :
	Component("Bomb"),
	Projectile(damagePoints, soundID),
	explosionTime(explosionTime),
	explosionKnockback(explosionKnockback),
	explosionID(explosionID),
	activationStatus(false)
{
}

std::ostream& operator<<(std::ostream& os, const BombComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.getDamagePoints() << ' ' << static_cast<std::size_t>(component.getSoundID())
		<< ' ' << component.explosionTime << ' ' << component.explosionID;

	return os;
}

float BombComponent::getExplosionTime() const
{
	return this->explosionTime;
}

float BombComponent::getExplosionKnockback() const
{
	return this->explosionKnockback;
}

std::string BombComponent::getExplosionID() const
{
	return this->explosionID;
}

void BombComponent::setExplosionTime(float explsionTime)
{
	this->explosionTime = explosionTime;
}

void BombComponent::setExplosionKnockback(float explosionKnockback)
{
	this->explosionKnockback = explosionKnockback;
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