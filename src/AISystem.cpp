/*
Copyright (c) 2017 InversePalindrome
Nihil - AISystem.cpp
InversePalindrome.com
*/


#include "AISystem.hpp"
#include "MathUtility.hpp"


AISystem::AISystem(Entities& entities, Events& events, Pathways& pathways) :
	System(entities, events),
	pathways(pathways)
{
	events.subscribe<entityplus::component_added<Entity, PatrolComponent>>([this](const auto& event) { addPathway(event.entity, event.component); });
}

void AISystem::update(float deltaTime)
{
	this->entities.for_each<PatrolComponent, PositionComponent>([this](auto entity, auto& patrol, auto& position)
	{
		this->updatePatrolling(entity, patrol, position);
	});
}

void AISystem::updatePatrolling(Entity entity, PatrolComponent& patrol, PositionComponent& position)
{
	const auto& waypoint = patrol.getCurrentWaypoint();
	const auto& entityPosition = position.getPosition();
	
	if (entityPosition.x > waypoint.x)
	{
		this->events.broadcast(DirectionChanged{ entity, Direction::Left });
	}
	else
	{
		this->events.broadcast(DirectionChanged{ entity, Direction::Right });
	}

	if(entityPosition.y > waypoint.y)
	{
		this->events.broadcast(Jumped{ entity });
	}
}

void AISystem::addPathway(Entity entity, PatrolComponent& patrol)
{
	if (entity.has_component<PositionComponent>() && this->getClosestPathway(entity.get_component<PositionComponent>().getPosition()).has_value())
	{
		patrol.setPathway(this->getClosestPathway(entity.get_component<PositionComponent>().getPosition()).value());
	}
}

std::optional<Pathway> AISystem::getClosestPathway(const sf::Vector2f& position)
{
	auto closestPathway = std::min_element(std::cbegin(this->pathways), std::cend(this->pathways), [&position](const auto& pathway1, const auto& pathway2) 
	{
		return MathUtils::distance(pathway1.second.getCurrentWaypoint(), position) < MathUtils::distance(pathway2.second.getCurrentWaypoint(), position);
	});
	

	if (closestPathway != std::cend(this->pathways))
	{
		return closestPathway->second;
	}
	else
	{
		return {};
	}
}