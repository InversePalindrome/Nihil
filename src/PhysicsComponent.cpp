/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsComponent.cpp
InversePalindrome.com
*/


#include "PhysicsComponent.hpp"

#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>


PhysicsComponent::PhysicsComponent(b2World& world, const b2Vec2& bodySize, b2BodyType bodyType, ObjectType objectType, std::int32_t collisionGroup) :
	PhysicsComponent(world, bodySize, bodyType, objectType, collisionGroup, 0.f, 0.f)
{
}

PhysicsComponent::PhysicsComponent(b2World& world, const b2Vec2& bodySize, b2BodyType bodyType, ObjectType objectType, std::int32_t collisionGroup,
	float maxVelocity, float accelerationRate)  :
	Component("PhysicsA"),
	body(nullptr),
	bodySize(bodySize),
	objectType(objectType),
	collisionGroup(collisionGroup),
	maxVelocity(maxVelocity),
	accelerationRate(accelerationRate)
{
	b2BodyDef bodyDefinition;
	bodyDefinition.type = bodyType;
	bodyDefinition.fixedRotation = true;
	
	b2PolygonShape fixtureShape;
	fixtureShape.SetAsBox(bodySize.x, bodySize.y);
	
 	b2FixtureDef fixture;
	fixture.shape = &fixtureShape;
	fixture.density = 1.f;
	fixture.friction = 0.3f;
	fixture.filter.groupIndex = collisionGroup;

	body = world.CreateBody(&bodyDefinition);
	body->CreateFixture(&fixture);
}

std::ostream& operator<<(std::ostream& os, const PhysicsComponent& component)
{
	os << component.getEntity() << ' ' << component.getName() << ' ' << component.bodySize.x << ' ' << component.bodySize.y <<  ' ' << ' ' << static_cast<std::size_t>(component.getType()) 
		<< ' ' << static_cast<std::size_t>(component.objectType) << ' ' << component.collisionGroup << ' ' << component.maxVelocity << ' ' << component.accelerationRate;

	return os;
}

b2Body* PhysicsComponent::getBody()
{
	return this->body;
}

b2Vec2 PhysicsComponent::getPosition() const
{
	return this->body->GetPosition();
}

b2Vec2 PhysicsComponent::getBodySize() const
{
	return this->bodySize;
}

b2Vec2 PhysicsComponent::getVelocity() const
{
	return this->body->GetLinearVelocity();
}

b2BodyType PhysicsComponent::getType() const
{
	return this->body->GetType();
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

std::int8_t PhysicsComponent::getCollisionGroup() const
{
	return this->collisionGroup;
}

void PhysicsComponent::setPosition(const b2Vec2& position)
{
	this->body->SetTransform(position, this->body->GetAngle());
}

void PhysicsComponent::setMaxVelocity(float maxVelocity)
{
	this->maxVelocity = maxVelocity;
}

void PhysicsComponent::setAccelerationRate(float accelerationRate)
{
	this->accelerationRate = accelerationRate;
}

void PhysicsComponent::setGravityScale(float gravityScale)
{
	this->body->SetGravityScale(gravityScale);
}

void PhysicsComponent::applyForce(const b2Vec2& force)
{
	this->body->ApplyForceToCenter(force, true);
}

void PhysicsComponent::applyImpulse(const b2Vec2& impulse)
{
	this->body->ApplyLinearImpulseToCenter(impulse, true);
}

ObjectType PhysicsComponent::getObjectType() const
{
	return this->objectType;
}