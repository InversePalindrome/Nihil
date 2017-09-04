/*
Copyright (c) 2017 InversePalindrome
Nihil - Component.cpp
InversePalindrome.com
*/


#include "Component.hpp"


Component::Component(const std::string& componentID) :
	name(componentID),
	entity(0u)
{
}

std::string Component::getName() const
{
	return this->name;
}

std::size_t Component::getEntity() const
{
	return this->entity;
}

void Component::setEntity(std::size_t entity)
{
	this->entity = entity;
}