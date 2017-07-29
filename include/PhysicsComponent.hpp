/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsComponent.hpp
InversePalindrome.com
*/


#pragma once

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>

#include <SFML/System/Vector2.hpp>


class PhysicsComponent
{
public:
	PhysicsComponent(b2World& world, const b2Vec2& bodySize,
		const b2Vec2& initialPosition, float maxVelocity, float accelerationRate);
	
	b2Vec2 getPosition() const;
	b2Vec2 getVelocity() const;
	float getMass() const;

	float getMaxVelocity() const;
	float getAccelerationRate() const;

	void setMaxVelocity(float maxVelocity);
	void setAccelerationRate(float accelerationRate);

	void applyImpulse(const b2Vec2& impulse);

private:
	b2Body* body;
	float maxVelocity;
	float accelerationRate;
};