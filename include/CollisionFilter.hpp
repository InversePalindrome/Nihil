/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionFilter.hpp
InversePalindrome.com
*/


#pragma once

#include "ObjectType.hpp"

#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>

#include <boost/functional/hash/hash.hpp>

#include <unordered_set>


struct CollisionPair;

class CollisionFilter : public b2ContactFilter
{
public:
	CollisionFilter();

private:
	std::unordered_set<std::pair<ObjectType, ObjectType>, boost::hash<std::pair<ObjectType, ObjectType>>> collisions;

	virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;
};
