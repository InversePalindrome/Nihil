/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsSystem.cpp
InversePalindrome.com
*/


#include "PhysicsSystem.hpp"
#include "PhysicsComponent.hpp"
#include "UnitConverter.hpp"


PhysicsSystem::PhysicsSystem(Entities& entities, Events& events, b2World& world) :
	System(entities, events),
	world(world)
{
	events.subscribe<DirectionChanged>([this](const auto& event) { moveEntity(event.entity, event.direction); });
	events.subscribe<Jumped>([this](const auto& event) { moveEntity(event.entity, Direction::Up); });
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
	
	switch (direction)
	{
	case Direction::Right:
		newVelocity.x = b2Min(currentVelocity.x + physics.getAccelerationRate(), physics.getMaxVelocity());
		this->events.broadcast(ChangeState{ entity, EntityState::Walking });
		break;
	case Direction::Left:
		newVelocity.x = b2Max(currentVelocity.x - physics.getAccelerationRate(), -physics.getMaxVelocity());
		this->events.broadcast(ChangeState{ entity, EntityState::Walking });
		break;
	case Direction::Up:
		const float jumpVelocity = 1.5 * physics.getMaxVelocity();
		newVelocity.y = jumpVelocity;
		this->events.broadcast(ChangeState{ entity, EntityState::Jumping });
		break;
	}

	const auto& deltaVelocity = newVelocity - currentVelocity;
	const auto& impulse = b2Vec2(deltaVelocity.x * physics.getMass(), deltaVelocity.y * physics.getMass());
	
	physics.applyImpulse(impulse);
}

void PhysicsSystem::convertPositionCoordinates(const PhysicsComponent& physics, PositionComponent& position)
{
	position.setPosition(sf::Vector2f(UnitConverter::metersToPixels(physics.getPosition().x), UnitConverter::metersToPixels(-physics.getPosition().y)));
}

void PhysicsSystem::checkIfStatic(Entity entity, const PhysicsComponent& physics)
{
	if (physics.getVelocity() == b2Vec2(0.f, 0.f))
	{
		this->events.broadcast(ChangeState{ entity, EntityState::Idle });
	}
}