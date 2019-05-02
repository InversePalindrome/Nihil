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

std::pair<float, float> Pathway::getRange() const
{
    return this->range;
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

    auto rangeElements = std::minmax_element(std::cbegin(this->waypoints), std::cend(this->waypoints),
        [](const auto & waypoint1, const auto & waypoint2) { return waypoint1.point.x < waypoint2.point.x; });

    this->range = { rangeElements.first->point.x, rangeElements.second->point.x };
}

bool Pathway::hasWaypoints() const
{
    return !this->waypoints.empty();
}