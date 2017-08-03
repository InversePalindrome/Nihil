/*
Copyright (c) 2017 InversePalindrome
Nihil - NeuralSystem.cpp
InversePalindrome.com
*/


#include "NeuralSystem.hpp"


NeuralSystem::NeuralSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	
}

void NeuralSystem::update(float deltaTime)
{
	this->updateTargetPosition();

	this->entities.for_each<AIComponent, PositionComponent>(
		[this](auto entity, auto& AI, const auto& position)
	{
		this->directAI(entity, AI, position);
	});
}

void NeuralSystem::directAI(Entity entity, AIComponent& AI, const PositionComponent& position)
{
	const auto& distanceFromPlayer = this->targetPosition.x - position.getPosition().x;
	
	if (std::abs(distanceFromPlayer) <= AI.getVisionRange())
	{
		if (distanceFromPlayer < 1.f)
		{
			this->events.broadcast(DirectionChanged{ entity, Direction::Left });
		}
		else
		{
			this->events.broadcast(DirectionChanged{ entity, Direction::Right });
		}
		return;
	}
	else
	{
		const auto& distanceToPathPoint = AI.getPath().first - position.getPosition().x;

		if (distanceToPathPoint < 0.f)
		{
			this->events.broadcast(DirectionChanged{ entity, Direction::Left });
		}
		else if(distanceToPathPoint > 2.f)
		{
			this->events.broadcast(DirectionChanged{ entity, Direction::Right });
		}
		else
		{
			AI.swapPathPoints();
		}
	}
}

void NeuralSystem::updateTargetPosition()
{
	this->entities.for_each<Controllable, PositionComponent>(
		[this](auto entity, auto& position)
	{
		this->targetPosition = position.getPosition();
	});
}