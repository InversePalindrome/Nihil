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
#include <unordered_map>


struct CollisionData
{
	CollisionData(b2Body* body, ObjectType objectType, const std::unordered_map<std::string, tmx::Property>& properties) :
		body(body),
		objectType(objectType),
		properties(properties)
	{
	}

	CollisionData(Entity entity, b2Body* body, ObjectType objectType) :
		entity(entity),
		body(body),
		objectType(objectType)
	{
	}

	Entity entity;
	b2Body* body;
	ObjectType objectType;
	std::unordered_map<std::string, tmx::Property> properties;
};

using CollisionsData = std::list<CollisionData>;