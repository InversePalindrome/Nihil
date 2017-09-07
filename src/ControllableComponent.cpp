/*
Copyright (c) 2017 InversePalindrome
Nihil - ControllableComponent.cpp
InversePalindrome.com
*/


#include "ControllableComponent.hpp"


ControllableComponent::ControllableComponent() :
	Component("Controllable")
{
}

std::ostream& operator<<(std::ostream& os, const ControllableComponent& component)
{
	os << component.getEntity() << ' ' << component.getName();

	return os;
}
