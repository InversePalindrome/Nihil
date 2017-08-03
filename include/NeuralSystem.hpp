/*
Copyright (c) 2017 InversePalindrome
Nihil - NeuralSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"


class NeuralSystem : public System
{
public:
	NeuralSystem(Entities& entities, Events& events);

	virtual void update(float deltaTime) override;

private:
	sf::Vector2f targetPosition;

	void directAI(Entity entity, AIComponent& AI, const PositionComponent& position);
	void updateTargetPosition();
};
