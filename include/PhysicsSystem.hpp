/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "Direction.hpp"

#include <Box2D/Dynamics/b2World.h>


class PhysicsSystem : public System
{
public:
	PhysicsSystem(Entities& entities, Events& events, b2World& world);

	virtual void update(float deltaTime) override;

private:
	b2World& world;

	void moveEntity(Direction direction);
	void convertPositionCoordinates(const PhysicsComponent& physics, PositionComponent& position);
};