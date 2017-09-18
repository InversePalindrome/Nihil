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

std::vector<Waypoint> Pathway::getWaypoints() const
{
	return this->waypoints;
}

Waypoint Pathway::getCurrentWaypoint() const
{
	return this->waypoints[this->currentWaypointIndex];
}

std::size_t Pathway::getCurrentWaypointIndex() const
{
	return this->currentWaypointIndex;
}

void Pathway::setCurrentWaypointIndex(std::size_t currentWaypointIndex)
{
	this->currentWaypointIndex = currentWaypointIndex;
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