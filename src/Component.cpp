/*
Copyright (c) 2017 InversePalindrome
Nihil - Component.cpp
InversePalindrome.com
*/


#include "Component.hpp"


Component::Component(const std::string& name) :
	name(name),
	entity(0)
{
}

std::string Component::getName() const
{
	return this->name;
}

std::int32_t Component::getEntityID() const
{
	return this->entity;
}


void Component::setName(const std::string& name)
{
	this->name = name;
}

void Component::setEntityID(std::int32_t entity)
{
	this->entity = entity;
}