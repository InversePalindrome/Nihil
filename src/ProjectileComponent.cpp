/*
Copyright (c) 2017 InversePalindrome
Nihil - ProjectileComponent.cpp
InversePalindrome.com
*/


#include "ProjectileComponent.hpp"


ProjectileComponent::ProjectileComponent(std::size_t damagePoints, float speed, SoundBuffersID soundID):
    Component("Projectile"),
	damagePoints(damagePoints),
	speed(speed),
	soundID(soundID)
{
}

std::ostream& operator<<(std::ostream& os, const ProjectileComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName();

	return os;
}

std::size_t ProjectileComponent::getDamagePoints() const
{
	return this->damagePoints;
}

float ProjectileComponent::getSpeed() const
{
	return this->speed;
}

SoundBuffersID ProjectileComponent::getSoundID() const
{
	return this->soundID;
}

void ProjectileComponent::setDamagePoints(std::size_t damagePoints)
{
	this->damagePoints = damagePoints;
}

void ProjectileComponent::setSpeed(float speed)
{
	this->speed = speed;
}

void ProjectileComponent::setSoundID(SoundBuffersID soundID)
{
	this->soundID = soundID;
}