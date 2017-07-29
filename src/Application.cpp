/*
Copyright (c) 2017 InversePalindrome
Nihil - Application.cpp
InversePalindrome.com
*/


#include "Application.hpp"
#include "SplashState.hpp"
#include "StartState.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "SettingsState.hpp"
#include "CharactersState.hpp"

#include <Thor/Resources/SfmlLoaders.hpp>

#include <SFML/Window/Event.hpp>

#include <chrono>


Application::Application() :
	window(sf::VideoMode(2048u, 1536u), "Nihil", sf::Style::Close | sf::Style::Titlebar),
	resourceManager("Resources/Files/ResourcePaths.txt"),
	guiManager(window),
	stateData(resourceManager, guiManager, inputHandler, window),
	stateMachine(stateData)
{
	stateMachine.registerState<SplashState>(StateID::Splash);
	stateMachine.registerState<StartState>(StateID::Start);
	stateMachine.registerState<MenuState>(StateID::Menu);
	stateMachine.registerState<GameState>(StateID::Game);
	stateMachine.registerState<SettingsState>(StateID::Settings);
	stateMachine.registerState<CharactersState>(StateID::Characters);

	stateMachine.pushState(StateID::Splash);
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
	this->stateMachine.update(deltaTime);
	this->guiManager.update(deltaTime);
}

void Application::render()
{
	this->window.clear();

	this->stateMachine.draw();
	this->guiManager.display();

	this->window.display();
}