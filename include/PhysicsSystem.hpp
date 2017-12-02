/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "Direction.hpp"
#include "Callbacks.hpp"
#include "CollisionData.hpp"

#include <Box2D/Dynamics/b2World.h>


class PhysicsSystem : public System
{
public:
	PhysicsSystem(Entities& entities, Events& events, b2World& world, CollisionsData& collisionsData);

	virtual void update(float deltaTime) override;

	void setEntitiesProperties(const EntityProperties& entitiesProperties);

private:
	b2World& world;

	void moveEntity(Entity entity, Direction direction);
	void stopEntity(Entity entity);
	void makeJump(Entity entity);

	void propelFromWater(Entity entity);

	void setGravityScale(Entity entity, float gravityScale);
	void setLinearDamping(Entity entity, float linearDamping);
	void setVelocity(Entity entity, Direction direction);
	void setFriction(Entity entity, ObjectType fixtureType, float friction);
	void setMidAirStatus(Entity entity, bool midAirStatus);

	void applyImpulse(Entity entity, const b2Vec2& impulse);
	void applyForce(Entity entity, const b2Vec2& force);

	void convertPositionCoordinates(const PhysicsComponent& physics, PositionComponent& position);
	void checkPhysicalStatus(Entity entity, PhysicsComponent& physics);

	void setUserData(Entity entity);

	CollisionsData& collisionsData;
	EntityProperties entitiesProperties;
	
	Callbacks callbacks;
};