/*
Copyright (c) 2017 InversePalindrome
Nihil - State.cpp
InversePalindrome.com
*/


#include "State.hpp"
#include "StateMachine.hpp"


State::State(StateMachine& stateMachine, StateData& stateData) :
	stateMachine(stateMachine),
	stateData(stateData),
	visibilityStatus(true)
{
}

void State::showWidgets(bool showStatus) const
{
}

bool State::isTransparent() const
{
	return false;
}

bool State::isVisible() const
{
	return this->visibilityStatus;
}

void State::setVisibility(bool visibilityStatus)
{
	this->visibilityStatus = visibilityStatus;
}