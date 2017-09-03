/*
Copyright (c) 2017 InversePalindrome
Nihil - StateComponent.cpp
InversePalindrome.com
*/


#include "StateComponent.hpp"


StateComponent::StateComponent() :
	Component(ComponentID::State),
	state(EntityState::Idle)
{
}

std::ostream& StateComponent::operator<<(std::ostream& os)
{
	os << static_cast<std::size_t>(this->state);

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