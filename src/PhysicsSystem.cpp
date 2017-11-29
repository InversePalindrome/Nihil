/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsSystem.cpp
InversePalindrome.com
*/


#include "PhysicsSystem.hpp"
#include "PhysicsComponent.hpp"
#include "UnitConverter.hpp"
#include "FrictionUtility.hpp"


PhysicsSystem::PhysicsSystem(Entities& entities, Events& events, b2World& world, CollisionsData& collisionsData) :
	System(entities, events),
	world(world),
	collisionsData(collisionsData)
{
	events.subscribe<entityplus::component_added<Entity, PhysicsComponent>>([this](const auto& event) { setInitialData(event.entity, event.component); });
	events.subscribe<DirectionChanged>([this](const auto& event) { moveEntity(event.entity, event.direction); });
	events.subscribe<Jumped>([this](const auto& event) { makeJump(event.entity); });
	events.subscribe<PropelFromWater>([this](const auto& event) { propelFromWater(event.entity); });
	events.subscribe<StopMovement>([this](const auto& event) { stopEntity(event.entity); });
	events.subscribe<ApplyForce>([this](const auto& event) { applyForce(event.entity, event.force); });
	events.subscribe<ApplyImpulse>([this](const auto& event) { applyImpulse(event.entity, event.impulse); });
	events.subscribe<SetGravityScale>([this](const auto& event) { setGravityScale(event.entity, event.gravity); });
	events.subscribe<SetLinearDamping>([this](const auto& event) { setLinearDamping(event.entity, event.linearDamping); });
	events.subscribe<SetVelocity>([this](const auto& event) { setVelocity(event.entity, event.direction); });
	events.subscribe<SetFriction>([this](const auto& event) { setFriction(event.entity, event.fixtureType, event.friction); });
	events.subscribe<SetMidAirStatus>([this](const auto& event) { setMidAirStatus(event.entity, event.midAirStatus); });
}

void PhysicsSystem::update(float deltaTime)
{
	this->entities.for_each<PhysicsComponent, PositionComponent>(
		[this](auto entity, auto& physics, auto& position) 
	{
		this->convertPositionCoordinates(physics, position);
		this->checkPhysicalStatus(entity, physics);
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
		newVelocity.x = b2Min(currentVelocity.x + physics.getAccelerationRate().x, physics.getMaxVelocity().x);
		deltaVelocity.x = newVelocity.x - currentVelocity.x;
		this->events.broadcast(ChangeState{ entity , EntityState::Walking });
		break;
	case Direction::Left:
		newVelocity.x = b2Max(currentVelocity.x - physics.getAccelerationRate().x, -physics.getMaxVelocity().x);
		deltaVelocity.x = newVelocity.x - currentVelocity.x;
		this->events.broadcast(ChangeState{ entity , EntityState::Walking });
		break;
	case Direction::Up:
		newVelocity.y = b2Min(currentVelocity.y + physics.getAccelerationRate().y, physics.getMaxVelocity().y);
		deltaVelocity.y = newVelocity.y - currentVelocity.y;
		break;
	case Direction::Down:
		newVelocity.y = b2Max(currentVelocity.y - physics.getAccelerationRate().y, -physics.getMaxVelocity().y);
		deltaVelocity.y = newVelocity.y - currentVelocity.y;
		break;
	}

	const b2Vec2 impulse({ deltaVelocity.x * physics.getMass(), deltaVelocity.y * physics.getMass() });

	physics.applyImpulse(impulse);
	physics.setDirection(direction);
}

void PhysicsSystem::stopEntity(Entity entity)
{
	if (entity.has_component<PhysicsComponent>())
	{
		entity.get_component<PhysicsComponent>().setVelocity({ 0.f, 0.f });
	}
}

void PhysicsSystem::makeJump(Entity entity)
{
	if (entity.has_component<PhysicsComponent>())
	{
		auto& physics = entity.get_component<PhysicsComponent>();

		if (!physics.isMidAir())
		{
			this->events.broadcast(ChangeState{ entity, EntityState::Jumping });

			const b2Vec2 impulse({ 0.f, physics.getJumpVelocity() * physics.getMass() });

			physics.applyImpulse(impulse);

			if (entity.has_component<ControllableComponent>())
			{
				this->events.broadcast(EmitSound{ SoundBuffersID::Jump, false });
			}
		}
	}
}

void PhysicsSystem::propelFromWater(Entity entity)
{
	if (entity.has_component<PhysicsComponent>())
	{
		auto& physics = entity.get_component<PhysicsComponent>();
		
		if (physics.getDirection() == Direction::Up)
		{
			const float forcePerMassRatio = 400.f;

			physics.applyForce({0.f, physics.getMass() * forcePerMassRatio});
		}
	}
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

void PhysicsSystem::setVelocity(Entity entity, Direction direction)
{
	if (entity.has_component<PhysicsComponent>())
	{
		auto& physics = entity.get_component<PhysicsComponent>();

		b2Vec2 velocity(0.f, 0.f);

		switch (direction)
		{
		case Direction::Up:
			velocity.y = physics.getMaxVelocity().y;
			break;
		case Direction::Down:
			velocity.y = -physics.getMaxVelocity().y;
			break;
		case Direction::Right:
			velocity.x = physics.getMaxVelocity().x;
			break;
		case Direction::Left:
			velocity.x = -physics.getMaxVelocity().x;
			break;
		case Direction::RightUp:
			velocity = { physics.getMaxVelocity().x, physics.getMaxVelocity().y };
			break;
		case Direction::RightDown:
			velocity = { physics.getMaxVelocity().x, -physics.getMaxVelocity().y };
			break;
		case Direction::LeftUp:
			velocity = { -physics.getMaxVelocity().x, physics.getMaxVelocity().y };
			break;
		case Direction::LeftDown:
			velocity = { -physics.getMaxVelocity().x, -physics.getMaxVelocity().y };
			break;
		}

		physics.setVelocity(velocity);
	}
}

void PhysicsSystem::setFriction(Entity entity, ObjectType fixtureType, float friction)
{
	if (entity.has_component<PhysicsComponent>())
	{
		entity.get_component<PhysicsComponent>().setFriction(fixtureType, friction);
	}
}

void PhysicsSystem::setMidAirStatus(Entity entity, bool midAirStatus)
{
	if (entity.has_component<PhysicsComponent>())
	{
		entity.get_component<PhysicsComponent>().setMidAirStatus(midAirStatus);
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
		position.setPosition({ UnitConverter::metersToPixels(physics.getPosition().x), UnitConverter::metersToPixels(-physics.getPosition().y) });
	}
}

void PhysicsSystem::checkPhysicalStatus(Entity entity, PhysicsComponent& physics)
{
	if (entity.has_component<StateComponent>())
	{
		const auto& state = entity.get_component<StateComponent>();

		if (state.getState() != EntityState::Swimming)
		{
			if (physics.getRelativeVelocity() == b2Vec2(0.f, 0.f))
			{
				this->events.broadcast(ChangeState{ entity, EntityState::Idle });
			}
		}
	}
	  
	if (physics.isColliding(ObjectType::Feet, ObjectType::Block))
	{
		Utility::setFriction(entity, 0.3f); 
		this->events.broadcast(SetMidAirStatus{ entity, false });
	}

	if (physics.isColliding(ObjectType::Head, ObjectType::Liquid))
	{
		this->events.broadcast(ChangeState{ entity , EntityState::Swimming });
		this->events.broadcast(SetGravityScale{ entity, 0.f });
		this->events.broadcast(SetLinearDamping{ entity, 1.f });
	}
}

void PhysicsSystem::setInitialData(Entity entity, PhysicsComponent& physics)
{
	if (entity.has_component<PositionComponent>())
	{
		const auto& position = entity.get_component<PositionComponent>().getPosition();

		physics.setPosition({ UnitConverter::pixelsToMeters(position.x), UnitConverter::pixelsToMeters(-position.y) });

		auto& fixtures = physics.getFixtures();

		for (auto& fixture : fixtures)
		{
			this->collisionsData.push_back(CollisionData(entity, fixture.second, fixture.first));
			fixture.second->SetUserData(&this->collisionsData.back());
		}
	}
	if (physics.getObjectType() == ObjectType::ActivationPlatform)
	{
		this->events.broadcast(SetAutomatedStatus{ entity, false });
	}
}