/*
Copyright (c) 2017 InversePalindrome
Nihil - ChildComponent.cpp
InversePalindrome.com
*/


#include "ChildComponent.hpp"


ChildComponent::ChildComponent() :
	ChildComponent(-1)
{
}

ChildComponent::ChildComponent(std::int32_t parentID) :
	Component("ChildB"),
	parentID(parentID)
{
} 

std::ostream& operator<<(std::ostream& os, const ChildComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.parentID;

	return os;
}

std::int32_t ChildComponent::getParentID() const
{
	return this->parentID;
}

const sf::Transform& ChildComponent::getTransform() const
{
	return *this->transform;
}

void ChildComponent::setParentID(std::int32_t parentID)
{
	this->parentID = parentID;
}

void ChildComponent::setTransform(const sf::Transform& transform)
{
	this->transform = &transform;
}