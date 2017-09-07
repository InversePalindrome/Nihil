/*
Copyright (c) 2017 InversePalindrome
Nihil - StateData.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "Game.hpp"
#include "GUIManager.hpp"
#include "InputHandler.hpp"
#include "SoundManager.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>


struct StateData
{
	StateData(std::vector<Game>& games, ResourceManager& resourceManager, SoundManager& soundManager, GUIManager& guiManager, InputHandler& inputHandler, sf::RenderWindow& window);

	std::vector<Game>& games;

	ResourceManager& resourceManager;
	SoundManager& soundManager;
	GUIManager& guiManager;
	InputHandler& inputHandler;

	sf::RenderWindow& window;
}; 
