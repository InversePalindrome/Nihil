/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionHandler.cpp
InversePalindrome.com
*/


#include "CollisionHandler.hpp"


CollisionHandler::CollisionHandler(Events& events) :
	events(events)
{
}

void CollisionHandler::BeginContact(b2Contact* contact)
{
	auto* objectA = static_cast<CollisionData*>(contact->GetFixtureA()->GetBody()->GetUserData());
	auto* objectB = static_cast<CollisionData*>(contact->GetFixtureB()->GetBody()->GetUserData());

	if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Movable, ObjectType::Border))
	{
		orderedCollision.value().first.get().entity.sync();

		this->events.broadcast(DestroyEntity{ orderedCollision.value().first.get().entity });
	}
	else if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Enemy))
	{
		orderedCollision.value().first.get().entity.sync();
		orderedCollision.value().second.get().entity.sync();

		this->events.broadcast(CombatOcurred{ orderedCollision.value().second.get().entity, orderedCollision.value().first.get().entity });
		this->events.broadcast(ApplyKnockback{ orderedCollision.value().second.get().entity, orderedCollision.value().first.get().entity });
		this->events.broadcast(ChangeState{ orderedCollision.value().second.get().entity, EntityState::Attacking });
	}
	else if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Portal))
	{
		this->events.broadcast(ChangeLevel{ orderedCollision.value().second.get().properties["Destination"].getStringValue() });
	}
	else if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Teleporter))
	{
		orderedCollision.value().first.get().entity.sync();

		this->events.broadcast(ChangePosition{ orderedCollision.value().first.get().entity,
		{orderedCollision.value().second.get().properties["xLocation"].getFloatValue(), orderedCollision.value().second.get().properties["yLocation"].getFloatValue()} });
	}
	else if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Pickup))
	{
		orderedCollision.value().first.get().entity.sync();
		orderedCollision.value().second.get().entity.sync();

		this->events.broadcast(PickedUpItem{ orderedCollision.value().first.get().entity, orderedCollision.value().second.get().entity });
	}
	else if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Alive , ObjectType::Trampoline))
	{
		orderedCollision.value().first.get().entity.sync();

		this->events.broadcast(ApplyImpulse{ orderedCollision.value().first.get().entity, b2Vec2(0.f, orderedCollision.value().second.get().properties["Impulse"].getFloatValue())});
	}
	else if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Enemy, ObjectType::Waypoint))
	{
		orderedCollision.value().first.get().entity.sync();

		this->events.broadcast(CrossedWaypoint{ orderedCollision.value().first.get().entity });
	}
	else if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Bullet, ObjectType::Alive))
	{
		orderedCollision.value().first.get().entity.sync();
		orderedCollision.value().second.get().entity.sync();

		this->events.broadcast(CombatOcurred{ orderedCollision.value().first.get().entity, orderedCollision.value().second.get().entity });
		this->events.broadcast(StopMovement{ orderedCollision.value().second.get().entity });
	}
	else if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Explosion, ObjectType::Alive))
	{
		orderedCollision.value().first.get().entity.sync();
		orderedCollision.value().second.get().entity.sync();

		this->events.broadcast(ApplyBlastImpact{ orderedCollision.value().first.get().entity, orderedCollision.value().second.get().entity });
	}
	else if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Character))
	{
		orderedCollision.value().first.get().entity.sync();
		orderedCollision.value().second.get().entity.sync();
		
		this->events.broadcast(CanConversate{ orderedCollision.value().first.get().entity, 
			orderedCollision.value().second.get().entity, true });
	}

	if (auto& collider = this->getCollider(objectA, objectB, ObjectType::Bullet))
	{
		collider.value().get().entity.sync();

		this->events.broadcast(DestroyEntity{ collider.value().get().entity });
	}
	else if (auto& collider = this->getCollider(objectA, objectB, ObjectType::Bomb))
	{
		collider.value().get().entity.sync();

		this->events.broadcast(ActivateBomb{ collider.value().get().entity });
	}
}

void CollisionHandler::EndContact(b2Contact* contact)
{
	auto* objectA = static_cast<CollisionData*>(contact->GetFixtureA()->GetBody()->GetUserData());
	auto* objectB = static_cast<CollisionData*>(contact->GetFixtureB()->GetBody()->GetUserData());

	if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Character))
	{
		orderedCollision.value().first.get().entity.sync();
		orderedCollision.value().second.get().entity.sync();

		this->events.broadcast(CanConversate{ orderedCollision.value().first.get().entity,
			orderedCollision.value().second.get().entity, false });

		this->events.broadcast(DisplayConversation{ orderedCollision.value().second.get().entity, false });
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