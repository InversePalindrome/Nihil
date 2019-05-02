/*
Copyright (c) 2017 InversePalindrome
Nihil - AISystem.cpp
InversePalindrome.com
*/


#include "AISystem.hpp"
#include "MathUtility.hpp"
#include "EntityUtility.hpp"
#include "CollisionData.hpp"

#include <limits>


AISystem::AISystem(Entities& entities, Events& events, Pathways& pathways) :
    System(entities, events),
    pathways(pathways)
{
    events.subscribe<entityplus::component_added<Entity, ControllableComponent>>([this](const auto & event)
        {
            targetEntity = event.entity;
        });

    events.subscribe<AddedUserData>([this](const auto & event) { addProperties(event.entity); });
    events.subscribe<CrossedWaypoint>([this](const auto & event) { changeWaypoint(event.entity); });
}

void AISystem::update(float deltaTime)
{
    if (const auto & targetPosition = this->getTargetPosition())
    {
        this->entities.for_each<AI, PatrolComponent, PositionComponent>([this, targetPosition](auto entity, auto & patrol, auto & position)
            {
                if (patrol.hasWaypoints())
                {
                    if (entity.has_component<ChaseComponent>() && this->isWithinRange(entity, position.getPosition(), targetPosition.value(), entity.get_component<ChaseComponent>().getVisionRange()))
                    {
                        this->chaseTarget(patrol, position.getPosition(), targetPosition.value());
                    }

                    this->updateMovement(entity, patrol, position.getPosition());
                }
            });

        this->entities.for_each<AI, RangeAttackComponent, PositionComponent, TimerComponent>([this, targetPosition](auto entity, auto & rangeAttack, auto & position, auto & timer)
            {
                if (timer.hasTimer("Reload") && timer.hasTimerExpired("Reload") && this->isFacingTarget(entity) &&
                    this->isWithinRange(entity, position.getPosition(), targetPosition.value(), rangeAttack.getAttackRange()))
                {
                    this->events.broadcast(ShootProjectile{ entity, rangeAttack.getProjectileID() });

                    timer.restartTimer("Reload");
                }
            });
    }
}

void AISystem::updateMovement(Entity entity, PatrolComponent& patrol, const sf::Vector2f& position)
{
    const auto& waypoint = patrol.getCurrentWaypoint();

    if (position.x > waypoint.point.x)
    {
        this->events.broadcast(ChangeDirection{ entity, Direction::Left });
    }
    else
    {
        this->events.broadcast(ChangeDirection{ entity, Direction::Right });
    }

    if (position.y > waypoint.point.y)
    {
        this->events.broadcast(Jumped{ entity });
    }
}

void AISystem::addProperties(Entity entity)
{
    if (entity.has_tag<Turret>() && entity.has_component<PhysicsComponent>())
    {
        auto& physics = entity.get_component<PhysicsComponent>();

        auto direction = static_cast<Direction>(static_cast<CollisionData*>(physics.getUserData(ObjectType::Tile))->properties["Direction"].getIntValue());

        physics.setDirection(direction);

        auto angle = static_cast<CollisionData*>(physics.getUserData(ObjectType::Tile))->properties["Angle"].getFloatValue();

        Utility::setAngle(entity, angle);
    }

    if (entity.has_component<PatrolComponent>())
    {
        if (auto pathway = this->getPathway(entity))
        {
            pathway.value().sortWaypoints();

            entity.get_component<PatrolComponent>().setPathway(pathway.value());
        }
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
    auto minDistance = std::numeric_limits<float>().max();

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

std::optional<Pathway> AISystem::getPathway(Entity entity)
{
    if (entity.has_component<PhysicsComponent>())
    {
        auto entityPathwayID = static_cast<std::size_t>(static_cast<CollisionData*>(entity.get_component<PhysicsComponent>()
            .getUserData(ObjectType::Enemy))->properties["PathwayID"].getIntValue());

        for (const auto& [pathwayID, pathway] : this->pathways)
        {
            if (pathwayID == entityPathwayID)
            {
                return pathway;
            }
        }
    }

    return {};
}

std::optional<sf::Vector2f> AISystem::getTargetPosition()
{
    if (this->targetEntity.sync() && targetEntity.has_component<PositionComponent>())
    {
        return this->targetEntity.get_component<PositionComponent>().getPosition();
    }

    return {};
}

bool AISystem::isWithinRange(Entity AI, const sf::Vector2f& AIPosition, const sf::Vector2f& targetPosition, float visionRange)
{
    bool isWithingPatrolRange = true;

    if (AI.has_component<PatrolComponent>())
    {
        const auto& [initialPosition, finalPosition] = AI.get_component<PatrolComponent>().getRange();

        if (targetPosition.x < initialPosition || targetPosition.x > finalPosition)
        {
            isWithingPatrolRange = false;
        }
    }

    return Utility::distance(AIPosition, targetPosition) <= visionRange && isWithingPatrolRange;
}

bool AISystem::isFacingTarget(Entity entity)
{
    if (this->targetEntity.sync() && this->targetEntity.has_component<PhysicsComponent>() && entity.has_component<PhysicsComponent>())
    {
        const auto& entityPhysics = entity.get_component<PhysicsComponent>();

        const auto& entityPosition = entityPhysics.getPosition();
        const auto& targetPosition = this->targetEntity.get_component<PhysicsComponent>().getPosition();

        return   (((entityPhysics.getDirection() == Direction::Right && targetPosition.x > entityPosition.x)
            || (entityPhysics.getDirection() == Direction::Left && targetPosition.x < entityPosition.x)) &&
            std::abs(entityPosition.y - targetPosition.y) <= entityPhysics.getBodySize().y)
            || entityPhysics.getDirection() == Direction::Up;
    }

    return false;
}