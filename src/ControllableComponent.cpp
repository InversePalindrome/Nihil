/*
Copyright (c) 2017 InversePalindrome
Nihil - ControllableComponent.cpp
InversePalindrome.com
*/


#include "ControllableComponent.hpp"


ControllableComponent::ControllableComponent() :
	Component("Controllable"),
	midAirStatus(false)
{
}

std::ostream& operator<<(std::ostream& os, const ControllableComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName();

	return os;
}

void ControllableComponent::setMidAirStatus(bool midAirStatus)
{
	this->midAirStatus = midAirStatus;
}

bool ControllableComponent::isMidAir() const
{
	return this->midAirStatus;
}