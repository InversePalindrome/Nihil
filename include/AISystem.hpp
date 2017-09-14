/*
Copyright (c) 2017 InversePalindrome
Nihil - AISystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "Pathway.hpp"

#include <optional>


class AISystem : public System
{
public:
	AISystem(Entities& entities, Events& events, Pathways& pathways);

	virtual void update(float deltaTime) override;

private:
	Pathways& pathways;

	void updatePatrolling(Entity entity, PatrolComponent& patrol, PositionComponent& position);
	void addPathway(Entity entity, PatrolComponent& patrol);

	std::optional<Pathway> getClosestPathway(const sf::Vector2f& position);
};