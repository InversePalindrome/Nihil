/*
Copyright (c) 2017 InversePalindrome
Nihil - Application.hpp
InversePalindrome.com
*/


#pragma once

#include "Game.hpp"
#include "StateData.hpp"
#include "StateMachine.hpp"
#include "InputHandler.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"
#include "GUIManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>
#include <string>


class Application
{
public:
    Application();
    Application(const Application& application) = delete;
    Application& operator=(const Application& application) = delete;

    void run();

private:
    std::vector<Game> games;

    InputHandler inputHandler;

    ResourceManager resourceManager;
    SoundManager soundManager;
    GUIManager guiManager;

    sf::RenderWindow window;

    StateData stateData;
    StateMachine stateMachine;

    void handleEvents();
    void update(float deltaTime);
    void render();

    void loadGames(const std::string& fileName);
};