/*
Copyright (c) 2017 InversePalindrome
Nihil - StateComponent.cpp
InversePalindrome.com
*/


#include "StateComponent.hpp"


StateComponent::StateComponent() :
	StateComponent(EntityState::Idle)
{
}

StateComponent::StateComponent(EntityState state) :
	state(state)
{
}

EntityState StateComponent::getState() const
{
	return this->state;
}

void StateComponent::setState(EntityState state)
{
	this->state = state;
}