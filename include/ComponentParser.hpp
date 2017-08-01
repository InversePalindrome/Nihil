/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentParser.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "ResourceManager.hpp"

#include <box2d/Dynamics/b2World.h>

#include <string>
#include <functional>
#include <unordered_map>


class ComponentParser
{
public:
	ComponentParser(Entities& entities, ResourceManager& resourceManager, b2World& world);
	
	void parseComponents(const std::string& filePath);

private:
	Entities& entities;
	ResourceManager& resourceManager;
	b2World& world;

	std::unordered_map<std::string, std::function<void(Entity&, const std::string&)>> componentParsers;
};