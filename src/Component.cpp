/*
Copyright (c) 2017 InversePalindrome
Nihil - Component.cpp
InversePalindrome.com
*/


#include "Component.hpp"


Component::Component(ComponentID componentID) :
	componentID(componentID)
{
}

ComponentID Component::getID() const
{
	return this->componentID;
}