/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Direction.hpp"
#include "ObjectType.hpp"

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>

#include <SFML/System/Vector2.hpp>

#include <unordered_map>


class PhysicsComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const PhysicsComponent& component);

public:
	PhysicsComponent(b2World& world, const b2Vec2& bodySize, b2BodyType physicalType, ObjectType objectType, 
		const b2Vec2& maxVelocity, const b2Vec2& accelerationRate, float jumpVelocity);

	b2Body* getBody();
	ObjectType getObjectType() const;
	std::unordered_map<ObjectType, b2Fixture*>& getFixtures();
	b2ContactEdge* getContactList();

	b2Vec2 getPosition() const;
	float getAngle() const;
	b2Vec2 getBodySize() const;
	b2Vec2 getVelocity() const;
	b2BodyType getType() const;
	void* getUserData(ObjectType fixtureObject) const;

	float getMass() const;
	b2Vec2 getMaxVelocity() const;
	b2Vec2 getRelativeVelocity() const;
	b2Vec2 getAccelerationRate() const;
	float getJumpVelocity() const;
	Direction getDirection() const;
	float getLinearDamping() const;

	void setPosition(const b2Vec2& position);
	void setAngle(float angle);
	void setVelocity(const b2Vec2& velocity);
	void setUserData(void* userData);
	void setType(b2BodyType type);
	void setMaxVelocity(const b2Vec2& maxVelocity);
	void setAccelerationRate(const b2Vec2& accelerationRate);
	void setJumpVelocity(float jumpVelocity);
	void setGravityScale(float gravityScale);
	void setDirection(Direction direction);
	void setLinearDamping(float linearDamping);
	void setFriction(ObjectType fixtureObject, float friction);
	void setMidAirStatus(bool midAirStatus);
	void setUnderWaterStatus(bool underWaterStatus);

	bool isColliding(ObjectType fixtureObject, ObjectType collidableObject) const;
	bool isIntersecting(const b2Vec2& point) const;
	bool isMidAir() const;
	bool isUnderWater() const;;

	void applyForce(const b2Vec2& force);
	void applyImpulse(const b2Vec2& impulse);

private:
	b2Body* body;
	std::unordered_map<ObjectType, b2Fixture*> fixtures;
	b2Vec2 bodySize;
	ObjectType objectType;

	b2Vec2 maxVelocity;
	b2Vec2 accelerationRate;
	float jumpVelocity;
	Direction direction;

	bool midAirStatus;
	bool underWaterStatus;
};

std::ostream& operator<<(std::ostream& os, const PhysicsComponent& component);