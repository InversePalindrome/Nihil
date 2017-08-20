/*
Copyright (c) 2017 InversePalindrome
Nihil - StateData.cpp
InversePalindrome.com
*/


#include "StateData.hpp"


StateData::StateData(ResourceManager& resourceManager, SoundManager& soundManager, GUIManager& guiManager, InputHandler& inputHandler, Events& events, sf::RenderWindow& window) :
	resourceManager(resourceManager),
	soundManager(soundManager),
	guiManager(guiManager),
	inputHandler(inputHandler),
	events(events),
	window(window)
{
}