/*
Copyright (c) 2017 InversePalindrome
Nihil - AISystem.cpp
InversePalindrome.com
*/


#include "AISystem.hpp"
#include "MathUtility.hpp"

#include <limits>


AISystem::AISystem(Entities& entities, Events& events, Pathways& pathways) :
	System(entities, events),
	pathways(pathways)
{
	events.subscribe<entityplus::component_added<Entity, PatrolComponent>>([this](auto& event)
	{ 
		callbacks.addCallback([this, event]()
		{
			addPathway(event.entity);
		});
	});

	events.subscribe<CrossedWaypoint>([this](const auto& event)
	{
		if (event.entity.has_component<PatrolComponent>() && (!event.entity.has_component<ChaseComponent>() || !isWithinRange(event.entity.get_component<PatrolComponent>().getRange(),
			event.entity.get_component<PositionComponent>().getPosition(), getTargetPosition(), event.entity.get_component<ChaseComponent>().getVisionRange())))
		{
			changeWaypoint(event.entity);
		}
	});
}

void AISystem::update(float deltaTime)
{
	this->entities.for_each<AI, PatrolComponent, PositionComponent>([this](auto entity, auto& patrol, auto& position)
	{
		if (patrol.hasWaypoints())
		{
			if (entity.has_component<ChaseComponent>() && this->isWithinRange(patrol.getRange(), position.getPosition(), this->getTargetPosition(), entity.get_component<ChaseComponent>().getVisionRange()))
			{
				this->chaseTarget(patrol, position.getPosition(), this->getTargetPosition());
			}

			this->updateMovement(entity, patrol, position.getPosition());
		}
	});

	this->entities.for_each<AI, RangeAttackComponent, ParentComponent, PositionComponent, TimerComponent>([this](auto entity, auto& rangeAttack, auto& parent, auto& position, auto& timer)
	{
		if (timer.hasTimer("Reload") && timer.hasTimerExpired("Reload") && ((entity.has_component<PatrolComponent>() && 
			this->isWithinRange(entity.get_component<PatrolComponent>().getRange(), position.getPosition(), this->getTargetPosition(), rangeAttack.getAttackRange())) ||
			this->isWithinRange(position.getPosition(), this->getTargetPosition(), rangeAttack.getAttackRange())))
		{
			this->events.broadcast(ShootProjectile{ entity, rangeAttack.getProjectileID(), this->getTargetPosition() });

			timer.restartTimer("Reload");
		}
	});
	
	this->callbacks.update();
	this->callbacks.clearCallbacks();
}

void AISystem::updateMovement(Entity entity, PatrolComponent& patrol, const sf::Vector2f& position)
{
	const auto& waypoint = patrol.getCurrentWaypoint();

	if (position.x > waypoint.point.x)
	{
		this->events.broadcast(DirectionChanged{ entity, Direction::Left });
	}
	else
	{
		this->events.broadcast(DirectionChanged{ entity, Direction::Right });
	}

	if (position.y > waypoint.point.y)
	{
		this->events.broadcast(Jumped{ entity });
	}
}

void AISystem::addPathway(Entity entity)
{
	entity.sync();

	if (!entity.has_component<PositionComponent>() || !entity.has_component<PatrolComponent>())
	{
		return;
	}

	if (auto pathway = this->getClosestPathway(entity.get_component<PositionComponent>().getPosition()))
	{
		pathway.value().sortWaypoints();
		
		entity.get_component<PatrolComponent>().setPathway(pathway.value());
	}
}

void AISystem::changeWaypoint(Entity entity)
{
	if (entity.has_component<PatrolComponent>())
	{
		entity.get_component<PatrolComponent>().moveToNextWaypoint();
	}
}

void AISystem::chaseTarget(PatrolComponent& patrol, const sf::Vector2f& AIPosition, const sf::Vector2f& targetPosition)
{
	const auto& waypoints = patrol.getPathway().getWaypoints();

	std::size_t waypointIndex = 0u;
	float minDistance = std::numeric_limits<float>().max();

	for (std::size_t i = 0u; i < waypoints.size(); ++i)
	{
		const auto& distance = MathUtils::distance(AIPosition, waypoints[i].point);
		
			if (targetPosition.x - AIPosition.x < 0.f)
			{
				if (waypoints[i].point.x < AIPosition.x && distance < minDistance)
				{
					waypointIndex = i;
					minDistance = distance;
				}
			}
			else
			{
				if (waypoints[i].point.x > AIPosition.x && distance < minDistance)
				{
					waypointIndex = i;
					minDistance = distance;
				}
			}
	}

	patrol.setCurrentWaypointIndex(waypointIndex);
}

std::optional<Pathway> AISystem::getClosestPathway(const sf::Vector2f& position)
{
	auto closestPathway = std::min_element(std::cbegin(this->pathways), std::cend(this->pathways), [&position](const auto& pathway1, const auto& pathway2) 
	{
		return MathUtils::distance(pathway1.second.getCurrentWaypoint().point, position) < MathUtils::distance(pathway2.second.getCurrentWaypoint().point, position);
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

sf::Vector2f AISystem::getTargetPosition() const
{
	sf::Vector2f targetPosition;

	this->entities.for_each<ControllableComponent, PositionComponent>([&targetPosition](auto entity, auto& controllable, auto& position) { targetPosition = position.getPosition(); });

	return targetPosition;
}

bool AISystem::isWithinRange(const sf::Vector2f& AIPosition, const sf::Vector2f& targetPosition, float visionRange)
{
	return MathUtils::distance(AIPosition, targetPosition) <= visionRange;
}

bool AISystem::isWithinRange(const std::pair<float, float>& patrolRange, const sf::Vector2f& AIPosition, const sf::Vector2f& targetPosition, float visionRange) const
{
	return MathUtils::distance(AIPosition, targetPosition) <= visionRange && 
		targetPosition.x > patrolRange.first && targetPosition.x < patrolRange.second;
}