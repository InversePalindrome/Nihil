/*
Copyright (c) 2017 InversePalindrome
Nihil - ParentComponent.cpp
InversePalindrome.com
*/


#include "ParentComponent.hpp"


ParentComponent::ParentComponent(std::size_t ID) :
	Component(ComponentID::Parent),
	ID(ID)
{
}

std::ostream& ParentComponent::operator<<(std::ostream& os)
{
	os << this->ID;

	return os;
}

std::size_t ParentComponent::getID() const
{
	return this->ID;
}

void ParentComponent::setID(std::size_t ID)
{
	this->ID = ID;
}