/*
Copyright (c) 2017 InversePalindrome
Nihil - PauseState.cpp
InversePalindrome.com
*/


#include "PauseState.hpp"
#include "StateMachine.hpp"


PauseState::PauseState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData)
{
}

void PauseState::handleEvent(const sf::Event& event)
{
	if (this->stateData.inputHandler.isActive(ActionID::ESC))
	{
		this->stateMachine.popState();
	}
}

void PauseState::update(float deltaTime)
{

}

void PauseState::draw()
{

}

bool PauseState::isTransparent() const
{
	return true;
}