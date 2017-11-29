/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionHandler.cpp
InversePalindrome.com
*/


#include "CollisionHandler.hpp"
#include "FrictionUtility.hpp"


CollisionHandler::CollisionHandler(Events& events) :
	events(events)
{
}

void CollisionHandler::BeginContact(b2Contact* contact)
{
	auto* objectA = static_cast<CollisionData*>(contact->GetFixtureA()->GetUserData());
	auto* objectB = static_cast<CollisionData*>(contact->GetFixtureB()->GetUserData());

	if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Border))
	{
		orderedCollision->first.get().entity.sync();

		this->events.broadcast(DestroyEntity{ orderedCollision->first.get().entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Checkpoint))
	{
		this->events.broadcast(CrossedCheckpoint{ { orderedCollision->second.get().properties["xPosition"].getFloatValue(), orderedCollision->second.get().properties["yPosition"].getFloatValue() } });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Enemy))
	{
		orderedCollision->first.get().entity.sync();
		orderedCollision->second.get().entity.sync();

		this->events.broadcast(CombatOcurred{ orderedCollision->second.get().entity, orderedCollision.value().first.get().entity });
		this->events.broadcast(ApplyKnockback{ orderedCollision->second.get().entity, orderedCollision.value().first.get().entity });
		this->events.broadcast(ChangeState{ orderedCollision->second.get().entity, EntityState::Attacking });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Portal))
	{
		this->events.broadcast(ChangeLevel{ orderedCollision->second.get().properties["Destination"].getStringValue(),
		{ orderedCollision->second.get().properties["xPosition"].getFloatValue(), orderedCollision->second.get().properties["yPosition"].getFloatValue() } });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Teleporter))
	{
		orderedCollision->first.get().entity.sync();

		this->events.broadcast(SetPosition{ orderedCollision->first.get().entity,
		{orderedCollision->second.get().properties["xPosition"].getFloatValue(), orderedCollision.value().second.get().properties["yPosition"].getFloatValue()} });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Pickup))
	{
		orderedCollision->first.get().entity.sync();
		orderedCollision->second.get().entity.sync();

		this->events.broadcast(PickedUpItem{ orderedCollision->first.get().entity, orderedCollision.value().second.get().entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Movable, ObjectType::Trampoline))
	{
		orderedCollision->first.get().entity.sync();

		this->events.broadcast(ApplyImpulse{ orderedCollision->first.get().entity, { 0.f, orderedCollision.value().second.get().properties["Impulse"].getFloatValue() } });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Waypoint))
	{
		orderedCollision->first.get().entity.sync();

		this->events.broadcast(CrossedWaypoint{ orderedCollision->first.get().entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Bullet, ObjectType::Alive))
	{
		orderedCollision->first.get().entity.sync();
		orderedCollision->second.get().entity.sync();

		this->events.broadcast(CombatOcurred{ orderedCollision->first.get().entity, orderedCollision.value().second.get().entity });
		this->events.broadcast(StopMovement{ orderedCollision->second.get().entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Explosion, ObjectType::Alive))
	{
		orderedCollision->first.get().entity.sync();
		orderedCollision->second.get().entity.sync();

		this->events.broadcast(ApplyBlastImpact{ orderedCollision->first.get().entity, orderedCollision->second.get().entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Character))
	{
		orderedCollision->second.get().entity.sync();

		this->events.broadcast(DisplayConversation{ orderedCollision->second.get().entity, true });
		this->events.broadcast(UpdateConversation{ orderedCollision->second.get().entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Spike))
	{
		orderedCollision->first.get().entity.sync();

		this->events.broadcast(DestroyEntity{ orderedCollision->first.get().entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Head, ObjectType::Liquid))
	{
		orderedCollision->first.get().entity.sync();
		
		this->events.broadcast(AddUnderWaterTimer{ orderedCollision->first.get().entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Block))
	{
		orderedCollision->first.get().entity.sync();
		
		this->events.broadcast(SetMidAirStatus{ orderedCollision->first.get().entity, false });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Ice))
	{
		orderedCollision->first.get().entity.sync();

		this->events.broadcast(SetMidAirStatus{ orderedCollision->first.get().entity, false });
		this->events.broadcast(SetFriction{ orderedCollision->first.get().entity, ObjectType::Player, 0.f });

		Utility::setFriction(&orderedCollision->first.get(), contact, 0.f);
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Character))
	{
		orderedCollision->second.get().entity.sync();

		this->events.broadcast(DisplayConversation{ orderedCollision->second.get().entity, true });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::ActivationPlatform))
	{
		orderedCollision->first.get().entity.sync();
		orderedCollision->second.get().entity.sync();

		this->events.broadcast(SetMidAirStatus{ orderedCollision->first.get().entity, false });
		this->events.broadcast(SetAutomatedStatus{ orderedCollision->second.get().entity, true });
	}

	if (const auto& collider = this->getCollider(objectA, objectB, ObjectType::Player))
	{
		collider->get().entity.sync();

		Utility::setFriction(&collider->get(), contact, 0.f);
	}
	if (const auto& collider = this->getCollider(objectA, objectB, ObjectType::Bullet))
	{
		collider->get().entity.sync();

		this->events.broadcast(DestroyEntity{ collider->get().entity });
	}
	else if (const auto& collider = this->getCollider(objectA, objectB, ObjectType::Bomb))
	{
		collider->get().entity.sync();

		this->events.broadcast(ActivateBomb{ collider->get().entity });
	}
}

void CollisionHandler::EndContact(b2Contact* contact)
{
	auto* objectA = static_cast<CollisionData*>(contact->GetFixtureA()->GetUserData());
	auto* objectB = static_cast<CollisionData*>(contact->GetFixtureB()->GetUserData());
	
	if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Character))
	{
		orderedCollision->second.get().entity.sync();
		
		this->events.broadcast(DisplayConversation{ orderedCollision->second.get().entity, false });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Block))
	{
		orderedCollision->first.get().entity.sync();
		
		this->events.broadcast(SetMidAirStatus{ orderedCollision->first.get().entity, true });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Ice))
	{
		orderedCollision->first.get().entity.sync();

		this->events.broadcast(SetMidAirStatus{ orderedCollision->first.get().entity, true });
		this->events.broadcast(SetFriction{ orderedCollision->first.get().entity, ObjectType::Player, 0.3f });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Head, ObjectType::Liquid))
	{
		orderedCollision->first.get().entity.sync();
		
		this->events.broadcast(RemoveUnderWaterTimer{ orderedCollision->first.get().entity });
		this->events.broadcast(ChangeState{ orderedCollision->first.get().entity, EntityState::Idle });
		this->events.broadcast(SetGravityScale{ orderedCollision->first.get().entity, 1.f });
		this->events.broadcast(SetLinearDamping{ orderedCollision->first.get().entity, 0.f });
		this->events.broadcast(PropelFromWater{ orderedCollision->first.get().entity });
	}
}

void CollisionHandler::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}

void CollisionHandler::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}

std::optional<CollisionHandler::Collider> CollisionHandler::getCollider(CollisionData* objectA, CollisionData* objectB, ObjectType type)
{
	if (objectA->objectType & type)
	{
		return Collider(*objectA);
	}
	else if (objectB->objectType & type)
	{
		return Collider(*objectB);
	}
	else
	{
		return {};
	}
}

std::optional<CollisionHandler::OrderedCollision> CollisionHandler::getOrderedCollision(CollisionData* objectA,
	CollisionData* objectB, ObjectType type1, ObjectType type2)
{
	if (objectA->objectType & type1 && objectB->objectType & type2)
	{
		return OrderedCollision(*objectA, *objectB);
	}
	else if (objectA->objectType & type2 && objectB->objectType & type1)
	{
		return OrderedCollision(*objectB, *objectA);
	}
	else
	{
		return {};
	}
}