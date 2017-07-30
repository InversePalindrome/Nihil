/*
Copyright (c) 2017 InversePalindrome
Nihil - ControlSystem.cpp
InversePalindrome.com
*/


#include "ControlSystem.hpp"
#include "EntityEvents.hpp"


ControlSystem::ControlSystem(Entities& entities, Events& events, InputHandler& inputHandler) :
	System(entities, events),
	inputHandler(inputHandler),
	timeSinceJump(0.f)
{
}

void ControlSystem::update(float deltaTime)
{
	entities.for_each<Controllable>(
		[this, deltaTime](auto entity)
	{
		this->reactToInput(deltaTime);
	});
}

void ControlSystem::reactToInput(float deltaTime)
{
	if (this->inputHandler.isActive(ActionID::Left))
	{
		this->events.broadcast(DirectionChanged{ Direction::Left });
	}
	else if (this->inputHandler.isActive(ActionID::Right))
	{
		this->events.broadcast(DirectionChanged{ Direction::Right });
	}
	else if (this->inputHandler.isActive(ActionID::Jump) && this->timeSinceJump > this->jumpInterval)
	{
		this->events.broadcast(DirectionChanged{ Direction::Up });
		this->timeSinceJump = 0.f;
	}

	this->timeSinceJump += deltaTime;
}