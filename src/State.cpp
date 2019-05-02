/*
Copyright (c) 2017 InversePalindrome
Nihil - State.cpp
InversePalindrome.com
*/


#include "State.hpp"
#include "StateMachine.hpp"


State::State(StateMachine& stateMachine, StateData& stateData) :
    stateMachine(stateMachine),
    stateData(stateData)
{
}

void State::showWidgets(bool showStatus)
{
}

bool State::isTransparent() const
{
    return false;
}

bool State::isDependent() const
{
    return false;
}