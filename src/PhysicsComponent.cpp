/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsComponent.cpp
InversePalindrome.com
*/


#include "PhysicsComponent.hpp"
#include "CollisionData.hpp"

#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>


PhysicsComponent::PhysicsComponent(b2World& world, const b2Vec2& bodySize, b2BodyType bodyType, ObjectType objectType,
    const b2Vec2& maxVelocity, const b2Vec2& accelerationRate, float jumpVelocity) :
    Component("Physics"),
    body(nullptr),
    bodySize(bodySize),
    objectType(objectType),
    maxVelocity(maxVelocity),
    jumpVelocity(jumpVelocity),
    accelerationRate(accelerationRate),
    direction(Direction::Right),
    midAirStatus(true),
    underWaterStatus(false)
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
    fixtureDef.restitution = 0.f;

    body = world.CreateBody(&bodyDefinition);

    switch (objectType)
    {
    case ObjectType::Pickup:
        fixtureDef.isSensor = true;
        break;
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
    case ObjectType::Enemy:
    {
        b2PolygonShape headShape;
        headShape.SetAsBox(bodySize.x, bodySize.y / 4.f, { 0.f, bodySize.y - bodySize.y / 4.f }, 0.f);

        b2FixtureDef headDef;
        headDef.isSensor = true;
        headDef.density = 0.f;
        headDef.shape = &headShape;

        b2PolygonShape feetShape;
        feetShape.SetAsBox(bodySize.x / 4.f, bodySize.y / 4.f, { 0.f, -bodySize.y + bodySize.y / 6.f }, 0.f);

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

std::ostream& operator<<(std::ostream & os, const PhysicsComponent & component)
{
    os << component.getEntityID() << ' ' << component.getName() << ' ' << component.bodySize.x << ' ' << component.bodySize.y << ' ' << static_cast<std::size_t>(component.getType())
        << ' ' << static_cast<std::size_t>(component.objectType) << ' ' << component.maxVelocity.x << ' ' << component.maxVelocity.y << ' ' << component.accelerationRate.x
        << ' ' << component.accelerationRate.y << ' ' << component.jumpVelocity;

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

float PhysicsComponent::getAngle() const
{
    return this->body->GetAngle();
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

void* PhysicsComponent::getUserData(ObjectType fixtureObject) const
{
    for (const auto* fixture = this->body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        const auto* userData = static_cast<CollisionData*>(fixture->GetUserData());

        if (userData->objectType == fixtureObject)
        {
            return fixture->GetUserData();
        }
    }

    return nullptr;
}

float PhysicsComponent::getMass() const
{
    return this->body->GetMass();
}

b2Vec2 PhysicsComponent::getMaxVelocity() const
{
    return this->maxVelocity;
}

b2Vec2 PhysicsComponent::getRelativeVelocity() const
{
    b2Vec2 relativeVelocity = this->getVelocity();

    for (const auto* edge = this->body->GetContactList(); edge; edge = edge->next)
    {
        if (edge->contact->IsTouching())
        {
            const auto* objectA = static_cast<CollisionData*>(edge->contact->GetFixtureA()->GetUserData());
            const auto* objectB = static_cast<CollisionData*>(edge->contact->GetFixtureB()->GetUserData());

            if (!objectA || !objectB)
            {
                return relativeVelocity;
            }

            if (objectA->objectType == ObjectType::Feet)
            {
                relativeVelocity.x -= objectB->fixture->GetBody()->GetLinearVelocity().x;
                relativeVelocity.y -= objectB->fixture->GetBody()->GetLinearVelocity().y;
            }
            else if (objectB->objectType == ObjectType::Feet)
            {
                relativeVelocity.x -= objectA->fixture->GetBody()->GetLinearVelocity().x;
                relativeVelocity.y -= objectA->fixture->GetBody()->GetLinearVelocity().y;
            }
        }
    }

    return relativeVelocity;
}

b2Vec2 PhysicsComponent::getAccelerationRate() const
{
    return this->accelerationRate;
}

float PhysicsComponent::getJumpVelocity() const
{
    return this->jumpVelocity;
}

Direction PhysicsComponent::getDirection() const
{
    return this->direction;
}

float PhysicsComponent::getLinearDamping() const
{
    return this->body->GetLinearDamping();
}

void PhysicsComponent::setPosition(const b2Vec2 & position)
{
    this->body->SetTransform(position, this->body->GetAngle());
}

void PhysicsComponent::setAngle(float angle)
{
    this->body->SetTransform(this->body->GetPosition(), angle);
}

void PhysicsComponent::setVelocity(const b2Vec2 & velocity)
{
    this->body->SetLinearVelocity(velocity);
}

void PhysicsComponent::setUserData(void* userData)
{
    this->body->SetUserData(userData);

    for (auto* fixture = this->body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        fixture->SetUserData(userData);
    }
}

void PhysicsComponent::setType(b2BodyType type)
{
    this->body->SetType(type);
}

void PhysicsComponent::setMaxVelocity(const b2Vec2 & maxVelocity)
{
    this->maxVelocity = maxVelocity;
}

void PhysicsComponent::setAccelerationRate(const b2Vec2 & accelerationRate)
{
    this->accelerationRate = accelerationRate;
}

void PhysicsComponent::setJumpVelocity(float jumpVelocity)
{
    this->jumpVelocity = jumpVelocity;
}

void PhysicsComponent::setGravityScale(float gravityScale)
{
    this->body->SetGravityScale(gravityScale);
}

void PhysicsComponent::setDirection(Direction direction)
{
    this->direction = direction;
}

void PhysicsComponent::setLinearDamping(float linearDamping)
{
    this->body->SetLinearDamping(linearDamping);
}

void PhysicsComponent::setFriction(ObjectType fixtureObject, float friction)
{
    for (auto* fixture = this->body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        auto* fixtureData = static_cast<CollisionData*>(fixture->GetUserData());

        if (fixtureData->objectType == fixtureObject)
        {
            fixture->SetFriction(friction);
        }
    }
}

void PhysicsComponent::setMidAirStatus(bool midAirStatus)
{
    this->midAirStatus = midAirStatus;
}

void PhysicsComponent::setUnderWaterStatus(bool underWaterStatus)
{
    this->underWaterStatus = underWaterStatus;
}

bool PhysicsComponent::isColliding(ObjectType fixtureObject, ObjectType collidableObject) const
{
    for (const auto* edge = this->body->GetContactList(); edge; edge = edge->next)
    {
        if (edge->contact->IsTouching())
        {
            const auto* objectA = static_cast<CollisionData*>(edge->contact->GetFixtureA()->GetUserData());
            const auto* objectB = static_cast<CollisionData*>(edge->contact->GetFixtureB()->GetUserData());

            if (!objectA || !objectB)
            {
                return false;
            }

            if ((objectA->objectType & fixtureObject && objectB->objectType & collidableObject) ||
                (objectA->objectType & collidableObject && objectB->objectType & fixtureObject))
            {
                return true;
            }
        }
    }

    return false;
}

bool PhysicsComponent::isIntersecting(const b2Vec2 & point) const
{
    for (const auto* fixture = this->body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        if (fixture->TestPoint(point))
        {
            return true;
        }
    }

    return false;
}

bool PhysicsComponent::isMidAir() const
{
    return this->midAirStatus;
}

bool PhysicsComponent::isUnderWater() const
{
    return this->underWaterStatus;
}

void PhysicsComponent::applyForce(const b2Vec2 & force)
{
    this->body->ApplyForceToCenter(force, true);
}

void PhysicsComponent::applyImpulse(const b2Vec2 & impulse)
{
    this->body->ApplyLinearImpulseToCenter(impulse, true);
}