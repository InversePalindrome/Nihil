/*
Copyright (c) 2017 InversePalindrome
Nihil - ControlSystem.cpp
InversePalindrome.com
*/


#include "ControlSystem.hpp"
#include "EntityEvents.hpp"


ControlSystem::ControlSystem(Entities& entities, Events& events, InputHandler& inputHandler) :
	System(entities, events),
	inputHandler(inputHandler)
{
}

void ControlSystem::update(float deltaTime)
{
	entities.for_each<Controllable>(
		[this](auto entity)
	{
		this->reactToInput();
	});
}

void ControlSystem::reactToInput()
{
	if (this->inputHandler.isActive(ActionID::Left))
	{
		this->events.broadcast(DirectionChanged{ Direction::Left });
	}
	else if (this->inputHandler.isActive(ActionID::Right))
	{
		this->events.broadcast(DirectionChanged{ Direction::Right });
	}
}