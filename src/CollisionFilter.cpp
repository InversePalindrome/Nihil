/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionFilter.cpp
InversePalindrome.com
*/


#include "CollisionFilter.hpp"
#include "CollisionData.hpp"
#include "FilePaths.hpp"

#include <fstream>


CollisionFilter::CollisionFilter()
{
	std::ifstream inFile(Path::miscellaneous / "Collisions.txt");

	std::size_t objectANum = 0u, objectBNum = 0u;

	while(inFile >> objectANum >> objectBNum)
	{
		auto objectA = static_cast<ObjectType>(objectANum);
		auto objectB = static_cast<ObjectType>(objectBNum);

		collisions.emplace(objectA, objectB);
		collisions.emplace(objectB, objectA);
	}
}

bool CollisionFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	auto* objectA = static_cast<CollisionData*>(fixtureA->GetUserData());
	auto* objectB = static_cast<CollisionData*>(fixtureB->GetUserData());

	if (collisions.count({ objectA->objectType, objectB->objectType }))
	{
		return false;
	}
	else
	{
		return true;
	}
}