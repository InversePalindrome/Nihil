/*
Copyright (c) 2017 InversePalindrome
Nihil - ProjectileComponent.cpp
InversePalindrome.com
*/


#include "ProjectileComponent.hpp"


ProjectileComponent::ProjectileComponent(float damagePoints, float speed):
    Component("Projectile"),
	damagePoints(damagePoints),
	speed(speed)
{
}

std::ostream& operator<<(std::ostream& os, const ProjectileComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName();

	return os;
}

float ProjectileComponent::getDamagePoints() const
{
	return this->damagePoints;
}

float ProjectileComponent::getSpeed() const
{
	return this->speed;
}

void ProjectileComponent::setDamagePoints(float damagePoints)
{
	this->damagePoints = damagePoints;
}

void ProjectileComponent::setSpeed(float speed)
{
	this->speed = speed;
}