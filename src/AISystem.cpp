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
	events.subscribe<entityplus::component_added<Entity, ControllableComponent>>([this](auto& event)
	{
		targetEntity = event.entity;
	});

	events.subscribe<entityplus::component_added<Entity, PatrolComponent>>([this](auto& event)
	{ 
		callbacks.addCallback([this, event]()
		{
			addPathway(event.entity);
		});
	});

	events.subscribe<CrossedWaypoint>([this](const auto& event)
	{
		if (const auto& targetPosition = getTargetPosition())
		{
			if (event.entity.has_component<PatrolComponent>() && (!event.entity.has_component<ChaseComponent>() || !isWithinRange(event.entity.get_component<PatrolComponent>().getRange(),
				event.entity.get_component<PositionComponent>().getPosition(), targetPosition.value(), event.entity.get_component<ChaseComponent>().getVisionRange())))
			{
				changeWaypoint(event.entity);
			}
		}
	});
}

void AISystem::update(float deltaTime)
{
	if (const auto& targetPosition = this->getTargetPosition())
	{
		this->entities.for_each<AI, PatrolComponent, PositionComponent>([this, targetPosition](auto entity, auto& patrol, auto& position)
		{
			if (patrol.hasWaypoints())
			{
				if (entity.has_component<ChaseComponent>() && this->isWithinRange(patrol.getRange(), position.getPosition(), targetPosition.value(), entity.get_component<ChaseComponent>().getVisionRange()))
				{
					this->chaseTarget(patrol, position.getPosition(), targetPosition.value());
				}

				this->updateMovement(entity, patrol, position.getPosition());
			}
		});

		this->entities.for_each<AI, RangeAttackComponent, ParentComponent, PositionComponent, TimerComponent>([this, targetPosition](auto entity, auto& rangeAttack, auto& parent, auto& position, auto& timer)
		{
			if (timer.hasTimer("Reload") && timer.hasTimerExpired("Reload") && this->isFacingTarget(entity) && ((entity.has_component<PatrolComponent>() &&
				this->isWithinRange(entity.get_component<PatrolComponent>().getRange(), position.getPosition(), targetPosition.value(), rangeAttack.getAttackRange())) ||
				this->isWithinRange(position.getPosition(), targetPosition.value(), rangeAttack.getAttackRange())))
			{
				this->events.broadcast(ShootProjectile{ entity, rangeAttack.getProjectileID(), targetPosition.value() });

				timer.restartTimer("Reload");
			}
		});
	}
	
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
		const auto& distance = Utility::distance(AIPosition, waypoints[i].point);
		
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
		return Utility::distance(pathway1.second.getCurrentWaypoint().point, position) < Utility::distance(pathway2.second.getCurrentWaypoint().point, position);
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

std::optional<sf::Vector2f> AISystem::getTargetPosition()
{
	if (this->targetEntity.sync() && targetEntity.has_component<PositionComponent>())
	{
		return this->targetEntity.get_component<PositionComponent>().getPosition();
	}
	else
	{
		return {};
	}
}

bool AISystem::isWithinRange(const sf::Vector2f& AIPosition, const sf::Vector2f& targetPosition, float visionRange)
{
	return Utility::distance(AIPosition, targetPosition) <= visionRange;
}

bool AISystem::isWithinRange(const std::pair<float, float>& patrolRange, const sf::Vector2f& AIPosition, const sf::Vector2f& targetPosition, float visionRange) const
{
	return Utility::distance(AIPosition, targetPosition) <= visionRange && 
		targetPosition.x > patrolRange.first && targetPosition.x < patrolRange.second;
}

bool AISystem::isFacingTarget(Entity entity)
{
	if (this->targetEntity.sync() && this->targetEntity.has_component<PositionComponent>() && this->targetEntity.has_component<PhysicsComponent>() &&
		entity.has_component<PositionComponent>() && entity.has_component<PhysicsComponent>())
	{
		const auto& targetPosition = this->targetEntity.get_component<PositionComponent>().getPosition();
		const auto& targetSize = this->targetEntity.get_component<PhysicsComponent>().getBodySize();

		const auto& entityPosition = entity.get_component<PositionComponent>().getPosition();

		auto entityDirection = entity.get_component<PhysicsComponent>().getDirection();

		return (std::abs(entityPosition.y - targetPosition.y) <= UnitConverter::metersToPixels(targetSize.y)) && (entityDirection == Direction::Right && targetPosition.x > entityPosition.x) 
			|| (entityDirection == Direction::Left && targetPosition.x < entityPosition.x);
	}
	else
	{
		return false;
	}
}