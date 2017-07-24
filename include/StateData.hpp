/*
Copyright (c) 2017 InversePalindrome
Nihil - StateData.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"
#include "GUIManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


struct StateData
{
	StateData(ResourceManager& resourceManager, GUIManager& guiManager, sf::RenderWindow& window);

	ResourceManager& resourceManager;

	sf::RenderWindow& window;
	GUIManager& guiManager;
}; 
