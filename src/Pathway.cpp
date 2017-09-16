/*
Copyright (c) 2017 InversePalindrome
Nihil - Pathway.cpp
InversePalindrome.com
*/


#include "Pathway.hpp"

#include <algorithm>


Waypoint::Waypoint(const sf::Vector2f& point, std::size_t step) :
	point(point),
	step(step)
{
}

bool Waypoint::operator<(const Waypoint& rhs) const
{
	return this->step < rhs.step;
}

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
	if (this->waypoints.size() > 1u)
	{
		++this->currentWaypointIndex;

		if (this->currentWaypointIndex == this->waypoints.size())
		{
			this->currentWaypointIndex = 1u;

			std::reverse(std::begin(this->waypoints), std::end(this->waypoints));
		}
	}
}

void Pathway::addWaypoint(const Waypoint& waypoint)
{
	this->waypoints.push_back(waypoint);
}

void Pathway::sortWaypoints()
{
	std::sort(std::begin(this->waypoints), std::end(this->waypoints));
}

bool Pathway::hasWaypoints() const
{
	return !this->waypoints.empty();
}