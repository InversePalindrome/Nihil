/*
Copyright (c) 2017 InversePalindrome
Nihil - GameState.cpp
InversePalindrome.com
*/


#include "GameState.hpp"
#include "StateMachine.hpp"


GameState::GameState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	map("Resources/Files/BackgroundTest.tmx", sf::Vector2f(2048, 1536))
{
	stateData.window.setView(stateData.window.getDefaultView());
}

void GameState::handleEvent(const sf::Event& event)
{

}

void GameState::update(float deltaTime)
{

}

void GameState::draw()
{
	this->stateData.window.draw(this->map);
}