/*
Copyright (c) 2017 InversePalindrome
Nihil - ProjectileComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"


class ProjectileComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const ProjectileComponent& component);

public:
	ProjectileComponent(float damagePoints, float speed);

	float getDamagePoints() const;
	float getSpeed() const;

	void setDamagePoints(float damagePoints);
	void setSpeed(float speed);

private:
	float damagePoints;
	float speed;
};

std::ostream& operator<<(std::ostream& os, const ProjectileComponent& component);