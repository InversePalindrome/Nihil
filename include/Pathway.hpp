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


struct Waypoint
{
	Waypoint(const sf::Vector2f& point, std::size_t step);

	bool operator<(const Waypoint& rhs) const;

	sf::Vector2f point;
	std::size_t step;
};

class Pathway
{
public:
	Pathway();

	std::vector<Waypoint> getWaypoints() const;
	Waypoint getCurrentWaypoint() const;
	std::size_t getCurrentWaypointIndex() const;
	std::pair<float, float> getRange() const;

	void setCurrentWaypointIndex(std::size_t currentWaypointIndex);

	void moveToNextWaypoint();
	void addWaypoint(const Waypoint& waypoint);

	void sortWaypoints();

	bool hasWaypoints() const;

private:
	std::vector<Waypoint> waypoints;
	std::size_t currentWaypointIndex;
	std::pair<float, float> range;
};

using Pathways = std::unordered_map<std::size_t, Pathway>;