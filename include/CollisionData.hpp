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


struct CollisionData
{
	CollisionData(b2Fixture* fixture, ObjectType objectType, const std::unordered_map<std::string, tmx::Property>& properties) :
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
	std::unordered_map<std::string, tmx::Property> properties;
};

using CollisionsData = std::list<CollisionData>;