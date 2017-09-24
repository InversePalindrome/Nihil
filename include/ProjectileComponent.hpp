/*
Copyright (c) 2017 InversePalindrome
Nihil - ProjectileComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "ResourceManager.hpp"


class ProjectileComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const ProjectileComponent& component);

public:
	ProjectileComponent(std::size_t damagePoints, float speed, SoundBuffersID soundID);

	std::size_t getDamagePoints() const;
	float getSpeed() const;
	SoundBuffersID getSoundID() const;

	void setDamagePoints(std::size_t damagePoints);
	void setSpeed(float speed);
	void setSoundID(SoundBuffersID soundID);

private:
	std::size_t damagePoints;
  	float speed;
	SoundBuffersID soundID;
};

std::ostream& operator<<(std::ostream& os, const ProjectileComponent& component);