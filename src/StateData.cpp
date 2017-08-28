/*
Copyright (c) 2017 InversePalindrome
Nihil - StateData.cpp
InversePalindrome.com
*/


#include "StateData.hpp"


StateData::StateData(Player& player, ResourceManager& resourceManager, SoundManager& soundManager, GUIManager& guiManager, InputHandler& inputHandler, sf::RenderWindow& window) :
	player(player),
	resourceManager(resourceManager),
	soundManager(soundManager),
	guiManager(guiManager),
	inputHandler(inputHandler),
	window(window)
{
}