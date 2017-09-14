/*
Copyright (c) 2017 InversePalindrome
Nihil - Pathway.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <cstddef>
#include <unordered_map>


using Waypoint = sf::Vector2f;

class Pathway
{
public:
	Pathway();

	Waypoint getCurrentWaypoint() const;

	void moveToNextWaypoint();
	void addWaypoint(const Waypoint& waypoint);

private:
	std::vector<Waypoint> waypoints;
	std::size_t currentWaypointIndex;
};

using Pathways = std::unordered_map<std::size_t, Pathway>;