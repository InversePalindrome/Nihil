/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsComponent.cpp
InversePalindrome.com
*/


#include "PhysicsComponent.hpp"

#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>


PhysicsComponent::PhysicsComponent(b2World& world, const b2Vec2& bodySize, const b2Vec2& initialPosition,
	float maxVelocity, float accelerationRate)  :
	body(nullptr),
	maxVelocity(maxVelocity),
	accelerationRate(accelerationRate)
{
	b2BodyDef bodyDefinition;
	bodyDefinition.type = b2_dynamicBody;
	bodyDefinition.position = initialPosition;
	bodyDefinition.fixedRotation = true;
	
	b2PolygonShape shape;
	shape.SetAsBox(bodySize.x, bodySize.y);
	
 	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.f;
	fixture.friction = 0.3f;

	body = world.CreateBody(&bodyDefinition);
	body->CreateFixture(&fixture);
}

b2Vec2 PhysicsComponent::getPosition() const
{
	return this->body->GetPosition();
}

b2Vec2 PhysicsComponent::getVelocity() const
{
	return this->body->GetLinearVelocity();
}

float PhysicsComponent::getMass() const
{
	return this->body->GetMass();
}

float PhysicsComponent::getMaxVelocity() const
{
	return this->maxVelocity;
}

float PhysicsComponent::getAccelerationRate() const
{
	return this->accelerationRate;
}

void PhysicsComponent::setMaxVelocity(float maxVelocity)
{
	this->maxVelocity = maxVelocity;
}

void PhysicsComponent::setAccelerationRate(float accelerationRate)
{
	this->accelerationRate = accelerationRate;
}

void PhysicsComponent::applyImpulse(const b2Vec2& impulse)
{
	this->body->ApplyLinearImpulse(impulse, this->body->GetPosition(), true);
}