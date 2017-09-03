/*
Copyright (c) 2017 InversePalindrome
Nihil - AIComponent.cpp
InversePalindrome.com
*/



#include "AIComponent.hpp"


AIComponent::AIComponent(float visionRange, float initialX, float finalX) :
	Component(ComponentID::AI),
	visionRange(visionRange),
	path(initialX, finalX)
{
}

std::ostream& AIComponent::operator<<(std::ostream& os)
{
	os << this->visionRange << ' ' << this->path.first << ' ' << this->path.second;

	return os;
}

float AIComponent::getVisionRange() const
{
	return this->visionRange;
}

AIComponent::Path AIComponent::getPath() const
{
	return this->path;
}

void AIComponent::setVisionRange(float visionRange)
{
	this->visionRange = visionRange;
}

void AIComponent::setPath(const Path& path)
{
	this->path = path;
}

void AIComponent::swapPathPoints()
{
	std::swap(this->path.first, this->path.second);
}