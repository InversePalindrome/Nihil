/*
Copyright (c) 2017 InversePalindrome
Nihil - Pathway.cpp
InversePalindrome.com
*/


#include "Pathway.hpp"


Pathway::Pathway() :
	currentWaypointIndex(0u)
{
}

Waypoint Pathway::getCurrentWaypoint() const
{
	return this->waypoints[this->currentWaypointIndex];
}

void Pathway::moveToNextWaypoint()
{
	++this->currentWaypointIndex;

	if (this->currentWaypointIndex == this->waypoints.size())
	{
		this->currentWaypointIndex = 0u;

		std::reverse(std::begin(this->waypoints), std::end(this->waypoints));
	}
}

void Pathway::addWaypoint(const Waypoint& waypoint)
{
	this->waypoints.push_back(waypoint);
}