/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsComponent.cpp
InversePalindrome.com
*/


#include "PhysicsComponent.hpp"
#include "CollisionData.hpp"

#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>


PhysicsComponent::PhysicsComponent(b2World& world, const b2Vec2& bodySize, b2BodyType bodyType, ObjectType objectType, std::int16_t collisionGroup,
	float maxVelocity, float jumpVelocity, float accelerationRate)  :
	Component("Physics"),
	body(nullptr),
	bodySize(bodySize),
	objectType(objectType),
	collisionGroup(collisionGroup),
	maxVelocity(maxVelocity),
	jumpVelocity(jumpVelocity),
	accelerationRate(accelerationRate),
	direction(Direction::Right)
{
	b2BodyDef bodyDefinition;
	bodyDefinition.type = bodyType;
	bodyDefinition.fixedRotation = true;
	
	b2PolygonShape fixtureShape;
	fixtureShape.SetAsBox(bodySize.x, bodySize.y);
	
 	b2FixtureDef fixtureDef;
	fixtureDef.shape = &fixtureShape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;
	fixtureDef.filter.groupIndex = collisionGroup;

	body = world.CreateBody(&bodyDefinition);

	switch (objectType)
	{
	case ObjectType::Platform:
		body->SetGravityScale(0.f);
		break;
	case ObjectType::Bullet:
		body->SetGravityScale(0.f);
		body->SetBullet(true);
		break;
	case ObjectType::Liquid:
		fixtureDef.isSensor = true;
		break;
	case ObjectType::Player:
	{
		b2PolygonShape headShape;
		headShape.SetAsBox(bodySize.x, bodySize.y / 4.f, { 0.f, bodySize.y - bodySize.y / 4.f }, 0.f);

		b2FixtureDef headDef;
		headDef.isSensor = true;
		headDef.density = 0.f;
		headDef.shape = &headShape;
		
		b2PolygonShape feetShape; 
		feetShape.SetAsBox(bodySize.x / 4.f, bodySize.y / 4.f, { 0.f, -bodySize.y }, 0.f);
		
		b2FixtureDef feetDef;
		feetDef.isSensor = true;
		feetDef.density = 0.f;
		feetDef.shape = &feetShape;

		fixtures[ObjectType::Head] = body->CreateFixture(&headDef);
		fixtures[ObjectType::Feet] = body->CreateFixture(&feetDef);
	}
		break;
	}

	fixtures[objectType] = body->CreateFixture(&fixtureDef);
}

std::ostream& operator<<(std::ostream& os, const PhysicsComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.bodySize.x << ' ' << component.bodySize.y <<  ' ' << static_cast<std::size_t>(component.getType()) 
		<< ' ' << static_cast<std::size_t>(component.objectType) << ' ' << component.collisionGroup << ' ' << component.maxVelocity << ' ' << component.jumpVelocity << ' ' << component.accelerationRate;

	return os;
}

b2Body* PhysicsComponent::getBody()
{
	return this->body;
}

ObjectType PhysicsComponent::getObjectType() const
{
	return this->objectType;
}

std::unordered_map<ObjectType, b2Fixture*>& PhysicsComponent::getFixtures()
{
	return this->fixtures;
}

b2ContactEdge* PhysicsComponent::getContactList()
{
	return this->body->GetContactList();
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

float PhysicsComponent::getJumpVelocity() const
{
	return this->jumpVelocity;
}

float PhysicsComponent::getAccelerationRate() const
{
	return this->accelerationRate;
}

Direction PhysicsComponent::getDirection() const
{
	return this->direction;
}

std::int16_t PhysicsComponent::getCollisionGroup() const
{
	return this->collisionGroup;
}

float PhysicsComponent::getLinearDamping() const
{
	return this->body->GetLinearDamping();
}

void PhysicsComponent::setPosition(const b2Vec2& position)
{
	this->body->SetTransform(position, this->body->GetAngle());
}

void PhysicsComponent::setVelocity(const b2Vec2& velocity)
{
	this->body->SetLinearVelocity(velocity);
}

void PhysicsComponent::setType(b2BodyType type)
{
	this->body->SetType(type);
}

void PhysicsComponent::setMaxVelocity(float maxVelocity)
{
	this->maxVelocity = maxVelocity;
}

void PhysicsComponent::setJumpVelocity(float jumpVelocity)
{
	this->jumpVelocity = jumpVelocity;
}

void PhysicsComponent::setAccelerationRate(float accelerationRate)
{
	this->accelerationRate = accelerationRate;
}

void PhysicsComponent::setGravityScale(float gravityScale)
{
	this->body->SetGravityScale(gravityScale);
}

void PhysicsComponent::setDirection(Direction direction)
{
	this->direction = direction;
}

void PhysicsComponent::setCollisionGroup(std::int16_t collisionGroup)
{
	this->collisionGroup = collisionGroup;

	for (auto& fixture : this->fixtures)
	{
		auto filterData = fixture.second->GetFilterData();

		filterData.groupIndex = collisionGroup;

		fixture.second->SetFilterData(filterData);
	}
}

void PhysicsComponent::setLinearDamping(float linearDamping)
{
	this->body->SetLinearDamping(linearDamping);
}

bool PhysicsComponent::isColliding(ObjectType fixtureObject, ObjectType collidableObject) const
{
	for (const auto* edge = this->body->GetContactList(); edge; edge = edge->next)
	{
		if (edge->contact->IsTouching())
		{
			const auto* objectA = static_cast<CollisionData*>(edge->contact->GetFixtureA()->GetUserData());
			const auto* objectB = static_cast<CollisionData*>(edge->contact->GetFixtureB()->GetUserData());

			if ((objectA->objectType & fixtureObject && objectB->objectType & collidableObject) ||
				(objectA->objectType & collidableObject && objectB->objectType & fixtureObject))
			{
				return true;
			}
		}
	}

	return false;
}

void PhysicsComponent::applyForce(const b2Vec2& force)
{
	this->body->ApplyForceToCenter(force, true);
}

void PhysicsComponent::applyImpulse(const b2Vec2& impulse)
{
	this->body->ApplyLinearImpulseToCenter(impulse, true);
}