/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionFilter.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "ObjectType.hpp"

#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>

#include <boost/functional/hash/hash.hpp>

#include <unordered_set>


struct CollisionPair;

class CollisionFilter : public b2ContactFilter
{
public:
	CollisionFilter(Events& events);

private:
	Events& events;
	std::unordered_set<std::pair<ObjectType, ObjectType>, boost::hash<std::pair<ObjectType, ObjectType>>> collisionTypes;
	std::unordered_set<std::pair<std::size_t, std::size_t>, boost::hash<std::pair<std::size_t, std::size_t>>> collisionIDs;

	virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;

	void manageCollisionIDs(Entity entityA, Entity entityB, bool collisionStatus);
};
