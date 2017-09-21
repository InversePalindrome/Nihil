/*
Copyright (c) 2017 InversePalindrome
Nihil - ParentComponent.cpp
InversePalindrome.com
*/


#include "ParentComponent.hpp"


ParentComponent::ParentComponent() :
	Component("ParentA"),
	childID(-1)
{
}

ParentComponent::ParentComponent(std::int32_t childID) :
	Component("ParentB"),
	childID(childID)
{
}

std::ostream& operator<<(std::ostream& os, const ParentComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.childID;

	return os;
}

std::int32_t ParentComponent::getChildID() const
{
	return this->childID;
}

void ParentComponent::setChildID(std::int32_t childID)
{
	this->childID = childID;
}