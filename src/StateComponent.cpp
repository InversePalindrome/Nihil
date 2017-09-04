/*
Copyright (c) 2017 InversePalindrome
Nihil - StateComponent.cpp
InversePalindrome.com
*/


#include "StateComponent.hpp"


StateComponent::StateComponent() :
	Component("State"),
	state(EntityState::Idle)
{
}

std::ostream& operator<<(std::ostream& os, const StateComponent& component)
{
	os << component.getEntity() << ' ' << component.getName() << ' ' << static_cast<std::size_t>(component.state);

	return os;
}

EntityState StateComponent::getState() const
{
	return this->state;
}

void StateComponent::setState(EntityState state)
{
	this->state = state;
}