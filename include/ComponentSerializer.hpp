/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentSerializer.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "Component.hpp"

#include <brigand/sequences/list.hpp>

#include <tuple>
#include <string>
#include <vector>
#include <cstddef>


class ComponentSerializer
{
public:
	ComponentSerializer(Entities& entities);

	void serialize(const std::string& pathFile);

	void saveBlueprint(const std::string& pathFile, const std::vector<std::tuple<std::int32_t, std::string, sf::Vector2f>>& entitiesFiles);

private:
	Entities& entities;
};