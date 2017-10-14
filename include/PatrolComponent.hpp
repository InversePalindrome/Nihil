/*
Copyright (c) 2017 InversePalindrome
Nihil - PatrolComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Pathway.hpp"


class PatrolComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const PatrolComponent& component);

public:
	PatrolComponent();

	Waypoint operator[](std::size_t index);

	Pathway getPathway() const;
	Waypoint getCurrentWaypoint() const;
	std::size_t getCurrentWaypointIndex() const;
	std::pair<float, float> getRange() const;

	void setPathway(const Pathway& pathway);
	void setCurrentWaypointIndex(std::size_t currentWaypointIndex);

	void moveToNextWaypoint();

	bool hasWaypoints() const;
	
private:
	Pathway pathway;
};

std::ostream& operator<<(std::ostream& os, const PatrolComponent& component);

