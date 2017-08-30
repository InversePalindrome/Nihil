/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionData.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "ObjectType.hpp"

#include <tmxlite/Property.hpp>

#include <Box2D/Dynamics/b2Body.h>

#include <list>


struct CollisionData
{
	CollisionData(b2Body* body, ObjectType objectType, const tmx::Property& properties) :
		body(body),
		objectType(objectType),
		properties(properties)
	{
	}

	CollisionData(Entity entity, b2Body* body, ObjectType objectType) :
		CollisionData(entity, body, objectType, {})
	{
	}

	CollisionData(Entity entity, b2Body* body, ObjectType objectType, const tmx::Property& properties) :
		entity(entity),
		body(body),
		objectType(objectType),
		properties(properties)
	{
	}

	Entity entity;
	b2Body* body;
	ObjectType objectType;
	tmx::Property properties;
};

using CollisionsData = std::list<CollisionData>;