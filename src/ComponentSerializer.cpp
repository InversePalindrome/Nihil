/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentSerializer.cpp
InversePalindrome.com
*/


#include "ComponentSerializer.hpp"

#include <brigand/algorithms/for_each.hpp>

#include <fstream>


ComponentSerializer::ComponentSerializer(Entities& entities) :
	entities(entities)
{
}

void ComponentSerializer::serialize(const std::string& pathFile)
{
	std::ofstream outFile(pathFile);

	brigand::for_each<ComponentList>([this, &outFile](auto componentType) 
	{
		using Type = decltype(componentType)::type;

		this->entities.for_each<Type>([&outFile](auto entity, auto& component)
		{
			outFile << component << '\n';
		});
	});
}

void ComponentSerializer::saveBlueprint(const std::string& pathFile, const std::vector<std::pair<std::string, sf::Vector2f>>& entitiesFile)
{
	std::ofstream outFile(pathFile);

	for (const auto& entityFile : entitiesFile)
	{
		outFile << entityFile.first << ' ' << entityFile.second.x << ' ' << entityFile.second.y << '\n';
	}
}
