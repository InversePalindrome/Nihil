/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentSerializer.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "Component.hpp"
#include "CollisionData.hpp"

#include <brigand/sequences/list.hpp>

#include <tuple>
#include <string>
#include <vector>
#include <cstddef>
#include <unordered_map>


class ComponentSerializer
{
public:
	ComponentSerializer(Entities& entities);

	EntityProperties& getProperties();

	void serialize(const std::string& fileName);

	void saveBlueprint(const std::string& fileName, const std::vector<std::tuple<std::int32_t, std::string, sf::Vector2f>>& entitiesData);

	void setProperties(const EntityProperties& properties);

private:
	Entities & entities;
	EntityProperties properties;
};