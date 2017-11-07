/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsSystem.cpp
InversePalindrome.com
*/


#include "PhysicsSystem.hpp"
#include "PhysicsComponent.hpp"
#include "UnitConverter.hpp"


PhysicsSystem::PhysicsSystem(Entities& entities, Events& events, b2World& world, CollisionsData& collisionsData) :
	System(entities, events),
	world(world),
	collisionsData(collisionsData)
{
	events.subscribe<entityplus::component_added<Entity, PhysicsComponent>>([this](const auto& event) { addInitialData(event.entity, event.component); });
	events.subscribe<DirectionChanged>([this](const auto& event) { changeEntityPosition(event.entity, event.direction); });
	events.subscribe<Jumped>([this](const auto& event) { makeJump(event.entity); });
	events.subscribe<StopMovement>([this](const auto& event) { stopEntity(event.entity); });
	events.subscribe<ApplyForce>([this](const auto& event) { applyForce(event.entity, event.force); });
	events.subscribe<ApplyImpulse>([this](const auto& event) { applyImpulse(event.entity, event.impulse); });
	events.subscribe<SetGravityScale>([this](const auto& event) { setGravityScale(event.entity, event.gravity); });
	events.subscribe<SetLinearDamping>([this](const auto& event) { setLinearDamping(event.entity, event.linearDamping); });
}

void PhysicsSystem::update(float deltaTime)
{
	this->entities.for_each<PhysicsComponent, PositionComponent>(
		[this](auto entity, const auto& physics, auto& position) 
	{
		this->convertPositionCoordinates(physics, position);
		this->checkPhysicalStatus(entity, physics);
	});
}

void PhysicsSystem::changeEntityPosition(Entity entity, Direction direction)
{
	auto& physics = entity.get_component<PhysicsComponent>();

	physics.setDirection(direction);

	const auto& currentVelocity = physics.getVelocity();
	
	b2Vec2 newVelocity(0.f, 0.f);
	b2Vec2 deltaVelocity(0.f, 0.f);
	
	switch (direction)
	{
	case Direction::Right:
		newVelocity.x = b2Min(currentVelocity.x + physics.getAccelerationRate(), physics.getMaxVelocity());
		deltaVelocity.x = newVelocity.x - currentVelocity.x;
		break;
	case Direction::Left:
		newVelocity.x = b2Max(currentVelocity.x - physics.getAccelerationRate(), -physics.getMaxVelocity());
		deltaVelocity.x = newVelocity.x - currentVelocity.x;
		break;
	}

	if (entity.has_component<StateComponent>())
	{
		this->events.broadcast(ChangeState{ entity, EntityState::Walking });
	}

	const auto& impulse = b2Vec2(deltaVelocity.x * physics.getMass(), deltaVelocity.y * physics.getMass());

	physics.applyImpulse(impulse);
}

void PhysicsSystem::stopEntity(Entity entity)
{
	entity.get_component<PhysicsComponent>().setVelocity(b2Vec2(0.f, 0.f));
}

void PhysicsSystem::makeJump(Entity entity)
{
	auto& physics = entity.get_component<PhysicsComponent>();

	this->events.broadcast(ChangeState{ entity, EntityState::Jumping });

	const auto& impulse = b2Vec2(0.f, physics.getJumpVelocity() * physics.getMass());

	physics.applyImpulse(impulse);
}


void PhysicsSystem::setGravityScale(Entity entity, float gravityScale)
{
	if (entity.has_component<PhysicsComponent>())
	{
		auto& physics = entity.get_component<PhysicsComponent>();

		physics.setGravityScale(gravityScale);
	}
}

void PhysicsSystem::setLinearDamping(Entity entity, float linearDamping)
{
	if (entity.has_component<PhysicsComponent>())
	{
		entity.get_component<PhysicsComponent>().setLinearDamping(linearDamping);
	}
}

void PhysicsSystem::applyImpulse(Entity entity, const b2Vec2& impulse)
{
	if (entity.has_component<PhysicsComponent>())
	{
		entity.get_component<PhysicsComponent>().applyImpulse(impulse);
	}
}

void PhysicsSystem::applyForce(Entity entity, const b2Vec2& force)
{
	if (entity.has_component<PhysicsComponent>())
	{
		entity.get_component<PhysicsComponent>().applyForce(force);
	}
}

void PhysicsSystem::convertPositionCoordinates(const PhysicsComponent& physics, PositionComponent& position)
{
	if (physics.getType() == b2BodyType::b2_dynamicBody || physics.getType() == b2BodyType::b2_kinematicBody)
	{
		position.setPosition(sf::Vector2f(UnitConverter::metersToPixels(physics.getPosition().x), UnitConverter::metersToPixels(-physics.getPosition().y)));
	}
}

void PhysicsSystem::checkPhysicalStatus(Entity entity, const PhysicsComponent& physics)
{
	if (entity.has_component<StateComponent>() && physics.getVelocity() == b2Vec2(0.f, 0.f))
	{
		this->events.broadcast(ChangeState{ entity, EntityState::Idle });
	}

	if (physics.getVelocity().y != 0.f)
	{
		this->events.broadcast(IsMidAir{ entity, true });
	}
	else
	{
		this->events.broadcast(IsMidAir{ entity, false });
	}
}

void PhysicsSystem::addInitialData(Entity entity, PhysicsComponent& physics)
{
	this->collisionsData.push_back(CollisionData(entity, physics.getBody(), physics.getObjectType()));
	this->collisionsData.back().body->SetUserData(&this->collisionsData.back());

	if (entity.has_component<PositionComponent>())
	{
		const auto& position = entity.get_component<PositionComponent>().getPosition();

		physics.setPosition(b2Vec2(UnitConverter::pixelsToMeters(position.x), UnitConverter::pixelsToMeters(-position.y)));
	}
}