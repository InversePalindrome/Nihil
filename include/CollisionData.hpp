/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionData.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "ObjectType.hpp"

#include <tmxlite/Property.hpp>

#include <Box2D/Dynamics/b2Fixture.h>

#include <list>
#include <unordered_map>


using Properties = std::unordered_map<std::string, tmx::Property>;
using EntityProperties = std::unordered_map<std::int32_t, Properties>;


struct CollisionData
{
	CollisionData(b2Fixture* fixture, ObjectType objectType, const Properties& properties) :
		fixture(fixture),
		objectType(objectType),
		properties(properties)
	{
	}

	CollisionData(Entity entity, b2Fixture* fixture, ObjectType objectType, const Properties& properties) :
		entity(entity),
		fixture(fixture),
		objectType(objectType),
		properties(properties)
	{
	}

	CollisionData(Entity entity, b2Fixture* fixture, ObjectType objectType) :
		entity(entity),
		fixture(fixture),
		objectType(objectType)
	{
	}

	Entity entity;
	b2Fixture* fixture;
	ObjectType objectType;
	Properties properties;
};

using CollisionsData = std::list<CollisionData>;