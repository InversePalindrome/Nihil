/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionData.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "ObjectType.hpp"

#include <Box2D/Dynamics/b2Body.h>

#include <list>


struct CollisionData
{
	CollisionData(b2Body& body, ObjectType objectType) :
		body(body),
		objectType(objectType)
	{
	}

	CollisionData(Entity entity, b2Body& body, ObjectType objectType) :
		entity(entity),
		body(body),
		objectType(objectType)
	{
	}

	Entity entity;
	b2Body& body;
	ObjectType objectType;
};

using CollisionsData = std::list<CollisionData>;