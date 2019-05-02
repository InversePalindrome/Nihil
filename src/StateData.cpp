/*
Copyright (c) 2017 InversePalindrome
Nihil - StateData.cpp
InversePalindrome.com
*/


#include "StateData.hpp"


StateData::StateData(std::vector<Game>& games, ResourceManager& resourceManager, SoundManager& soundManager,
    GUIManager& guiManager, InputHandler& inputHandler, sf::RenderWindow& window) :
    games(games),
    resourceManager(resourceManager),
    soundManager(soundManager),
    guiManager(guiManager),
    inputHandler(inputHandler),
    window(window)
{
}