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
	
	if (auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Trap))
	{
		
	}
	else if(auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Enemy))
	{
		orderedCollision.value().first.get().entity.sync();
		orderedCollision.value().second.get().entity.sync();

     	this->events.broadcast(CombatOcurred{ orderedCollision.value().second.get().entity, orderedCollision.value().first.get().entity });
		this->events.broadcast(ChangeState{ orderedCollision.value().second.get().entity, EntityState::Attacking });
	}
}

void CollisionHandler::EndContact(b2Contact* contact)
{
}

void CollisionHandler::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}

void CollisionHandler::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) 
{

}

std::optional<CollisionHandler::OrderedCollision> CollisionHandler::getOrderedCollision(CollisionData* objectA, 
	CollisionData* objectB, ObjectType type1, ObjectType type2)
{
	if (objectA->objectType == type1 && objectB->objectType == type2)
	{
		return OrderedCollision(*objectA, *objectB);
	}
	else if (objectA->objectType == type2 && objectB->objectType == type1)
	{
		return OrderedCollision(*objectB, *objectA);
	}
	else
	{
		return {};
	}
}