/*
Copyright (c) 2017 InversePalindrome
Nihil - GameState.cpp
InversePalindrome.com
*/


#include "GameState.hpp"
#include "StateMachine.hpp"


GameState::GameState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	entityManager(stateData.resourceManager),
	map("Resources/Files/Level1.tmx", sf::Vector2f(2048.f, 1536.f))
{
	stateData.window.setView(stateData.window.getDefaultView());
}

void GameState::handleEvent(const sf::Event& event)
{

}

void GameState::update(float deltaTime)
{
	this->entityManager.update(deltaTime);
}

void GameState::draw()
{
	this->stateData.window.draw(this->map);
	this->entityManager.draw(this->stateData.window);
}