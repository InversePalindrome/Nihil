/*
Copyright (c) 2017 InversePalindrome
Nihil - BulletComponent.cpp
InversePalindrome.com
*/


#include "BulletComponent.hpp"


BulletComponent::BulletComponent(std::int32_t damagePoints, SoundBuffersID soundID, float force) :
	Component("Bullet"),
	Projectile(damagePoints, soundID),
	force(force)
{
}

std::ostream& operator<<(std::ostream& os, const BulletComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.getDamagePoints() << ' ' << static_cast<std::size_t>(component.getSoundID()) 
		<< ' ' << component.force;

	return os;
}

float BulletComponent::getForce() const
{
	return this->force;
}

void BulletComponent::setForce(float force)
{
	this->force = force;
}