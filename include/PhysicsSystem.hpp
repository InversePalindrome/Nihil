/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "Direction.hpp"
#include "CollisionData.hpp"

#include <Box2D/Dynamics/b2World.h>


class PhysicsSystem : public System
{
public:
	PhysicsSystem(Entities& entities, Events& events, b2World& world, CollisionsData& collisionsData);

	virtual void update(float deltaTime) override;

private:
	b2World& world;
	CollisionsData& collisionsData;

	void moveEntity(Entity entity, Direction direction);
	void stopEntity(Entity entity);
	void makeJump(Entity entity);

	void applyImpulse(Entity entity, const b2Vec2& impulse);

	void convertPositionCoordinates(const PhysicsComponent& physics, PositionComponent& position);
	void checkIfStatic(Entity entity, const PhysicsComponent& physics);

	void addInitialData(Entity entity, PhysicsComponent& physics);
};