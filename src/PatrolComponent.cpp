/*
Copyright (c) 2017 InversePalindrome
Nihil - PatrolComponent.cpp
InversePalindrome.com
*/



#include "PatrolComponent.hpp"


PatrolComponent::PatrolComponent(float initialX, float finalX) :
	Component("Patrol"),
	path(initialX, finalX)
{
}

std::ostream& operator<<(std::ostream& os, const PatrolComponent& component)
{
	os  << component.getEntity() << ' ' << component.getName() << ' ' << 
		component.path.first << ' ' << component.path.second;

	return os;
}

PatrolComponent::Path PatrolComponent::getPath() const
{
	return this->path;
}

void PatrolComponent::setPath(const Path& path)
{
	this->path = path;
}

void PatrolComponent::swapPathPoints()
{
	std::swap(this->path.first, this->path.second);
}