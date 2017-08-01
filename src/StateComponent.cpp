/*
Copyright (c) 2017 InversePalindrome
Nihil - StateComponent.cpp
InversePalindrome.com
*/


#include "StateComponent.hpp"


StateComponent::StateComponent() :
	state(EntityState::Idle)
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