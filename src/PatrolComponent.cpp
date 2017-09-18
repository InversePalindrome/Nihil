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

Waypoint PatrolComponent::operator[](std::size_t index)
{
	return this->pathway.getWaypoints()[index];
}

Pathway PatrolComponent::getPathway() const
{
	return this->pathway;
}

Waypoint PatrolComponent::getCurrentWaypoint() const
{
	return this->pathway.getCurrentWaypoint();
}

std::size_t PatrolComponent::getCurrentWaypointIndex() const
{
	return this->pathway.getCurrentWaypointIndex();
}

void PatrolComponent::setPathway(const Pathway& pathway)
{
	this->pathway = pathway;
}

void PatrolComponent::setCurrentWaypointIndex(std::size_t currentWaypointIndex)
{
	this->pathway.setCurrentWaypointIndex(currentWaypointIndex);
}

void PatrolComponent::moveToNextWaypoint()
{
	this->pathway.moveToNextWaypoint();
}

bool PatrolComponent::hasWaypoints() const
{
	return this->pathway.hasWaypoints();
}