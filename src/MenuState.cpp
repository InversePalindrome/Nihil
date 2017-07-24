/*
Copyright (c) 2017 InversePalindrome
Nihil - MenuState.cpp
InversePalindrome.com
*/


#include "MenuState.hpp"
#include "StateMachine.hpp"


MenuState::MenuState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData)
{
}

void MenuState::handleEvent(const sf::Event& event)
{

}

void MenuState::update(float deltaTime)
{
	
}

void MenuState::draw()
{
}