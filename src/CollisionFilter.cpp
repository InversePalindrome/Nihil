/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionFilter.cpp
InversePalindrome.com
*/


#include "CollisionFilter.hpp"
#include "CollisionData.hpp"
#include "FilePaths.hpp"

#include <fstream>


CollisionFilter::CollisionFilter(Events& events) :
	events(events)
{
	std::ifstream inFile(Path::miscellaneous / "Collisions.txt");

	std::size_t objectANum = 0u, objectBNum = 0u;

	while(inFile >> objectANum >> objectBNum)
	{
		auto objectA = static_cast<ObjectType>(objectANum);
		auto objectB = static_cast<ObjectType>(objectBNum);

		collisionTypes.emplace(objectA, objectB);
		collisionTypes.emplace(objectB, objectA);
	}

	events.subscribe<ManageCollision>([this](const auto& event) { manageCollisionIDs(event.entityA, event.entityB, event.collisionStatus); });
}

bool CollisionFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	auto* objectA = static_cast<CollisionData*>(fixtureA->GetUserData());
	auto* objectB = static_cast<CollisionData*>(fixtureB->GetUserData());

	if (!objectA || !objectB)
	{
		return false;
	}

	if (objectA->isEntity && objectB->isEntity && objectA->entity.has_component<PositionComponent>() && objectB->entity.has_component<PositionComponent>())
	{
		auto entityAID = objectA->entity.get_component<PositionComponent>().getEntityID();
		auto entityBID = objectB->entity.get_component<PositionComponent>().getEntityID();

		if (this->collisionIDs.count({ entityAID, entityBID }))
		{
			return false;
		}
	}

	if(this->collisionTypes.count({ objectA->objectType, objectB->objectType }))
	{
		return false;
	}
	
	return true;
}

void CollisionFilter::manageCollisionIDs(Entity entityA, Entity entityB, bool collisionStatus)
{
	if (entityA.has_component<PositionComponent>() && entityB.has_component<PositionComponent>())
	{
		auto entityAID = entityA.get_component<PositionComponent>().getEntityID();
		auto entityBID = entityB.get_component<PositionComponent>().getEntityID();

		if (!collisionStatus)
		{
			this->collisionIDs.emplace(entityAID, entityBID);
			this->collisionIDs.emplace(entityBID, entityAID);
		}
		else
		{
			this->collisionIDs.erase({ entityAID, entityBID });
			this->collisionIDs.erase({ entityBID, entityAID });
		}
	}
}