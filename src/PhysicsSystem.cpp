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
	events.subscribe<DirectionChanged>([this](const auto& event) { moveEntity(event.entity, event.direction); });
	events.subscribe<Jumped>([this](const auto& event) { makeJump(event.entity); });
	events.subscribe<StopMovement>([this](const auto& event) { stopEntity(event.entity); });
	events.subscribe<CombatOcurred>([this](const auto& event) { applyImpulse(event.victim, b2Vec2(event.attacker.get_component<PhysicsComponent>().getVelocity().x * 50.f, 0.f)); });
	events.subscribe<TouchedTrampoline>([this](const auto& event) { applyImpulse(event.entity, b2Vec2(0.f, 250.f)); });
}

void PhysicsSystem::update(float deltaTime)
{
	this->entities.for_each<PhysicsComponent, PositionComponent>(
		[this](auto entity, const auto& physics, auto& position) 
	{
		this->convertPositionCoordinates(physics, position);
		this->checkIfStatic(entity, physics);
	});
}

void PhysicsSystem::moveEntity(Entity entity, Direction direction)
{
	auto& physics = entity.get_component<PhysicsComponent>();

	const auto& currentVelocity = physics.getVelocity();
	
	b2Vec2 newVelocity(0.f, 0.f);
	b2Vec2 deltaVelocity(0.f, 0.f);
	
	switch (direction)
	{
	case Direction::Right:
		newVelocity.x = b2Min(currentVelocity.x + physics.getAccelerationRate(), physics.getMaxVelocity());
		deltaVelocity.x = newVelocity.x - currentVelocity.x;
		this->events.broadcast(ChangeState{ entity, EntityState::Walking });
		break;
	case Direction::Left:
		newVelocity.x = b2Max(currentVelocity.x - physics.getAccelerationRate(), -physics.getMaxVelocity());
		deltaVelocity.x = newVelocity.x - currentVelocity.x;
		this->events.broadcast(ChangeState{ entity, EntityState::Walking });
		break;
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

	const auto& currentVelocity = physics.getVelocity();

	b2Vec2 newVelocity(0.f, 0.f);

	const float jumpVelocity = 15.f;
	newVelocity.y = jumpVelocity;

	this->events.broadcast(ChangeState{ entity, EntityState::Jumping });

	const auto& deltaVelocity = newVelocity - currentVelocity;

	const auto& impulse = b2Vec2(0.f, deltaVelocity.y * physics.getMass());

	physics.applyImpulse(impulse);
}

void PhysicsSystem::applyImpulse(Entity entity, const b2Vec2& impulse)
{
	if (entity.has_component<PhysicsComponent>())
	{
		entity.get_component<PhysicsComponent>().applyImpulse(impulse);
	}
}

void PhysicsSystem::convertPositionCoordinates(const PhysicsComponent& physics, PositionComponent& position)
{
	if (physics.getType() == b2BodyType::b2_dynamicBody || physics.getType() == b2BodyType::b2_kinematicBody)
	{
		position.setPosition(sf::Vector2f(UnitConverter::metersToPixels(physics.getPosition().x), UnitConverter::metersToPixels(-physics.getPosition().y)));
	}
}

void PhysicsSystem::checkIfStatic(Entity entity, const PhysicsComponent& physics)
{
	if (physics.getVelocity() == b2Vec2(0.f, 0.f))
	{
		this->events.broadcast(ChangeState{ entity, EntityState::Idle });
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