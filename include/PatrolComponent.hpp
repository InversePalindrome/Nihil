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

	void setPathway(const Pathway& pathway);

	Waypoint getCurrentWaypoint() const;

	void moveToNextWaypoint();

	bool hasWaypoints() const;
	
private:
	Pathway pathway;
};

std::ostream& operator<<(std::ostream& os, const PatrolComponent& component);

