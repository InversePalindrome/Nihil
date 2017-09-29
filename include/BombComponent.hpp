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
	BombComponent(std::size_t damagePoints, SoundBuffersID soundID, float explosionTime, float explosionKnockback, const std::string& explosionID);

	float getExplosionTime() const;
	float getExplosionKnockback() const;
	std::string getExplosionID() const;

	void setExplosionTime(float explsionTime);
	void setExplosionKnockback(float explosionKnockback);
	void setExplosionID(const std::string& explosionID);
	void setActivationStatus(bool explosionStatus);

	bool hasActivated() const;

private:
	float explosionTime;
	float explosionKnockback;
	std::string explosionID;
	bool activationStatus;
};

std::ostream& operator<<(std::ostream& os, const BombComponent& component);