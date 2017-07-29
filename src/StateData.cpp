/*
Copyright (c) 2017 InversePalindrome
Nihil - StateData.cpp
InversePalindrome.com
*/


#include "StateData.hpp"


StateData::StateData(ResourceManager& resourceManager, GUIManager& guiManager, InputHandler& inputHandler, sf::RenderWindow& window) :
	resourceManager(resourceManager),
	guiManager(guiManager),
	inputHandler(inputHandler),
	window(window)
{
}