/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentSerializer.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "Component.hpp"

#include <brigand/sequences/list.hpp>

#include <string>
#include <vector>


class ComponentSerializer
{
public:
	ComponentSerializer(Entities& entities);

	void serialize(const std::string& pathFile);

	void createBlueprint(const std::string& pathFile, const std::vector<std::pair<std::string, sf::Vector2f>>& entitiesFiles);

private:
	Entities& entities;
};