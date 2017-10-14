/*
Copyright (c) 2017 InversePalindrome
Nihil - AISystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "Pathway.hpp"
#include "Callbacks.hpp"

#include <SFML/System/Vector2.hpp>

#include <optional>


class AISystem : public System
{
public:
	AISystem(Entities& entities, Events& events, Pathways& pathways);

	virtual void update(float deltaTime) override;

private:
	Pathways& pathways;
	Callbacks callbacks;

	void updateMovement(Entity entity, PatrolComponent& patrol, const sf::Vector2f& position);

	void addPathway(Entity entity);

	void changeWaypoint(Entity entity);
	void chaseTarget(PatrolComponent& patrol, const sf::Vector2f& AIPosition, const sf::Vector2f& targetPosition);

	std::optional<Pathway> getClosestPathway(const sf::Vector2f& position);
	sf::Vector2f getTargetPosition() const;

	bool isWithinRange(const std::pair<float, float>& patrolRange, const sf::Vector2f& AIPosition, const sf::Vector2f& targetPosition,
		float visionRange) const;
};