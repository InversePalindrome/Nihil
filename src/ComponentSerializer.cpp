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
	
	brigand::for_each<ComponentList>([this](auto x) 
	{
		this->entities.for_each<decltype(x)::type>([](auto entity, auto& component) 
		{
			
		});
	});
}
