/*
Copyright (c) 2017 InversePalindrome
Nihil - BombComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Projectile.hpp"


class BombComponent : public Component, public Projectile
{
	friend std::ostream& operator<<(std::ostream& os, const BombComponent& component);

public:
	BombComponent(std::size_t damagePoints, SoundBuffersID soundID, float explosionTime, const std::string& explosionID);

	float getExplosionTime() const;
	std::string getExplosionID() const;

	void setExplosionTime(float explsionTime);
	void setExplosionID(const std::string& explosionID);
	void setActivationStatus(bool explosionStatus);

	bool hasActivated() const;

private:
	float explosionTime;
	std::string explosionID;
	bool activationStatus;
};

std::ostream& operator<<(std::ostream& os, const BombComponent& component);