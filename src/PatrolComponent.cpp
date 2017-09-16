/*
Copyright (c) 2017 InversePalindrome
Nihil - PatrolComponent.cpp
InversePalindrome.com
*/



#include "PatrolComponent.hpp"


PatrolComponent::PatrolComponent() :
	Component("Patrol")
{
}

std::ostream& operator<<(std::ostream& os, const PatrolComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName();

	return os;
}

void PatrolComponent::setPathway(const Pathway& pathway)
{
	this->pathway = pathway;
}

Waypoint PatrolComponent::getCurrentWaypoint() const
{
	return this->pathway.getCurrentWaypoint();
}

void PatrolComponent::moveToNextWaypoint()
{
	this->pathway.moveToNextWaypoint();
}

bool PatrolComponent::hasWaypoints() const
{
	return this->pathway.hasWaypoints();
}