/*
Copyright (c) 2017 InversePalindrome
Nihil - StateData.hpp
InversePalindrome.com
*/


#pragma once

#include "GUIManager.hpp"
#include "InputHandler.hpp"
#include "SoundManager.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


struct StateData
{
	StateData(ResourceManager& resourceManager, SoundManager& soundManager, GUIManager& guiManager, InputHandler& inputHandler, sf::RenderWindow& window);

	ResourceManager& resourceManager;
	SoundManager& soundManager;
	GUIManager& guiManager;
	InputHandler& inputHandler;

	sf::RenderWindow& window;
}; 
