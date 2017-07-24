/*
Copyright (c) 2017 InversePalindrome
Nihil - Application.hpp
InversePalindrome.com
*/


#pragma once

#include "StateData.hpp"
#include "StateMachine.hpp"
#include "ResourceManager.hpp"
#include "GUIManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


class Application
{
public:
	Application();

	void run();

private:
	StateData stateData;
	StateMachine stateMachine;

	ResourceManager resourceManager;
	GUIManager guiManager;

	sf::RenderWindow window;

	void handleEvents();
	void update(float deltaTime);
	void render();

	void loadResources();
};