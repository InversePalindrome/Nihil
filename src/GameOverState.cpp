/*
Copyright (c) 2017 InversePalindrome
Nihil - GameOverState.cpp
InversePalindrome.com
*/


#include "GameOverState.hpp"


GameOverState::GameOverState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData)
{
}

void GameOverState::handleEvent(const sf::Event& event)
{

}

void GameOverState::update(float deltaTime)
{

}

void GameOverState::draw()
{

}

bool GameOverState::isTransparent() const
{
	return true;
}