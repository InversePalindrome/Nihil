/*
Copyright (c) 2017 InversePalindrome
Nihil - Application.cpp
InversePalindrome.com
*/


#include "Application.hpp"
#include "SplashState.hpp"
#include "StartState.hpp"
#include "MenuState.hpp"
#include "HubState.hpp"
#include "GameState.hpp"
#include "SettingsState.hpp"
#include "ShopState.hpp"
#include "PauseState.hpp"
#include "AchievementsState.hpp"
#include "FilePaths.hpp"

#include <Thor/Resources/SfmlLoaders.hpp>

#include <SFML/Window/Event.hpp>

#include <chrono>
#include <fstream>
#include <sstream>


Application::Application() :
    window(sf::VideoMode(2048u, 1536u), "Nihil", sf::Style::Close | sf::Style::Titlebar),
    resourceManager("ResourcePaths.txt"),
    soundManager(resourceManager),
    guiManager(window),
    stateData(games, resourceManager, soundManager, guiManager, inputHandler, window),
    stateMachine(stateData)
{
    stateData.window.resetGLStates();

    stateMachine.registerState<SplashState>(StateID::Splash);
    stateMachine.registerState<StartState>(StateID::Start);
    stateMachine.registerState<MenuState>(StateID::Menu);
    stateMachine.registerState<GameState>(StateID::Game);
    stateMachine.registerState<HubState>(StateID::Hub);
    stateMachine.registerState<AchievementsState>(StateID::Achievements);
    stateMachine.registerState<SettingsState>(StateID::Settings);
    stateMachine.registerState<ShopState>(StateID::Shop);
    stateMachine.registerState<PauseState>(StateID::Pause);

    stateMachine.pushState(StateID::Splash);

    loadGames("SavedGames.txt");
}

void Application::run()
{
    std::chrono::high_resolution_clock clock;

    auto lastTime = clock.now();

    while (this->window.isOpen())
    {
        std::chrono::duration<float> deltaTime = clock.now() - lastTime;

        lastTime = clock.now();

        handleEvents();
        update(deltaTime.count());
        render();
    }
}

void Application::handleEvents()
{
    sf::Event event;

    this->inputHandler.clearEvents();

    while (this->window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            this->window.close();
            break;
        }

        this->inputHandler.pushEvent(event);
        this->stateMachine.handleEvent(event);
        this->guiManager.handleEvent(event);
    }
}

void Application::update(float deltaTime)
{
    this->inputHandler.update(this->window);
    this->inputHandler.invokeCallbacks();

    this->stateMachine.update(deltaTime);
    this->guiManager.update(deltaTime);
    this->soundManager.update();
}

void Application::render()
{
    this->window.clear();

    this->stateMachine.draw();
    this->guiManager.display();

    this->window.display();
}

void Application::loadGames(const std::string& fileName)
{
    std::ifstream inFile(Path::games / fileName);
    std::string gameData;
    std::string line;

    while (std::getline(inFile, line))
    {
        if (line.empty())
        {
            this->games.push_back(Game(gameData));
            gameData.clear();

            continue;
        }

        gameData += line + '\n';
    }
}