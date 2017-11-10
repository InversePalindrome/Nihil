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
	PhysicsComponent(b2World& world, const b2Vec2& bodySize, b2BodyType physicalType, ObjectType objectType, std::int16_t collisionGroup,
		float maxVelocity, float jumpVelocity, float accelerationRate);

	b2Body* getBody();
	ObjectType getObjectType() const;
	std::unordered_map<ObjectType, b2Fixture*>& getFixtures();
	b2ContactEdge* getContactList();

	b2Vec2 getPosition() const;
	b2Vec2 getBodySize() const;
	b2Vec2 getVelocity() const;
	b2BodyType getType() const;

	float getMass() const;
	float getMaxVelocity() const;
	float getJumpVelocity() const;
	float getAccelerationRate() const;
	Direction getDirection() const;
	std::int16_t getCollisionGroup() const;
	float getLinearDamping() const;

	void setPosition(const b2Vec2& position);
	void setVelocity(const b2Vec2& velocity);
	void setType(b2BodyType type);
	void setMaxVelocity(float maxVelocity);
	void setJumpVelocity(float jumpVelocity);
	void setAccelerationRate(float accelerationRate);
	void setGravityScale(float gravityScale);
	void setDirection(Direction direction);
	void setCollisionGroup(std::int16_t collisionGroup);
	void setLinearDamping(float linearDamping);

	bool isColliding(ObjectType fixtureObject, ObjectType collidableObject) const;

	void applyForce(const b2Vec2& force);
	void applyImpulse(const b2Vec2& impulse);

private:
	b2Body* body;
	std::unordered_map<ObjectType, b2Fixture*> fixtures;
	b2Vec2 bodySize;
	ObjectType objectType;
	std::int16_t collisionGroup;

	float maxVelocity;
	float jumpVelocity;
	float accelerationRate;
	Direction direction;
};

std::ostream& operator<<(std::ostream& os, const PhysicsComponent& component);