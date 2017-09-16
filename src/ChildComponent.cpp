/*
Copyright (c) 2017 InversePalindrome
Nihil - ChildComponent.cpp
InversePalindrome.com
*/


#include "ChildComponent.hpp"


ChildComponent::ChildComponent(std::size_t parentID) :
	Component("Child"),
	parentID(parentID)
{
}

std::ostream& operator<<(std::ostream& os, const ChildComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.parentID;

	return os;
}

std::size_t ChildComponent::getParentID() const
{
	return this->parentID;
}

const sf::Transform& ChildComponent::getTransform() const
{
	
	return *this->transform;
}

void ChildComponent::setParentID(std::size_t parentID)
{
	this->parentID = parentID;
}

void ChildComponent::setTransform(const sf::Transform& transform)
{
	this->transform = &transform;
}