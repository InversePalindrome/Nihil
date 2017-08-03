/*
Copyright (c) 2017 InversePalindrome
Nihil - ControlSystem.cpp
InversePalindrome.com
*/


#include "ControlSystem.hpp"


ControlSystem::ControlSystem(Entities& entities, Events& events, InputHandler& inputHandler) :
	System(entities, events),
	inputHandler(inputHandler),
	timeSinceJump(0.f)
{
}

void ControlSystem::update(float deltaTime)
{
	this->entities.for_each<Controllable>(
		[this, deltaTime](auto entity)
	{
		this->reactToInput(entity, deltaTime);
	});
}

void ControlSystem::reactToInput(Entity entity, float deltaTime)
{
	if (this->inputHandler.isActive(ActionID::Left))
	{
		this->events.broadcast(DirectionChanged{ entity, Direction::Left });
	}
	else if (this->inputHandler.isActive(ActionID::Right))
	{
		this->events.broadcast(DirectionChanged{ entity, Direction::Right });
	}
	else if (this->inputHandler.isActive(ActionID::Jump) && this->timeSinceJump > this->jumpInterval)
	{
		this->events.broadcast(Jumped{ entity });
		this->timeSinceJump = 0.f;
	}

	this->timeSinceJump += deltaTime;
}