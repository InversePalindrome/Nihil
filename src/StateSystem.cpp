/*
Copyright (c) 2017 InversePalindrome
Nihil - StateSystem.cpp
InversePalindrome.com
*/


#include "StateSystem.hpp"



StateSystem::StateSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	events.subscribe<ChangeState>([this](const auto& event) { changeState(event.entity, event.state); });
	events.subscribe<DirectionChanged>([this](const auto& event) { changeStateOnDirection(event.entity, event.direction); });
}

void StateSystem::update(float deltaTime)
{
	
}

void StateSystem::changeState(Entity entity, EntityState state)
{
	if (entity.has_component<StateComponent>())
	{
		auto& stateComponent = entity.get_component<StateComponent>();

		if (stateComponent.getState() != state)
		{
			stateComponent.setState(state);

			this->events.broadcast(StateChanged{ entity, state });
		}
	}
}

void StateSystem::changeStateOnDirection(Entity entity, Direction direction)
{
	if (entity.has_component<StateComponent>())
	{
		if (entity.get_component<StateComponent>().getState() != EntityState::Swimming && (direction == Direction::Left || direction == Direction::Right))
		{
			this->changeState(entity, EntityState::Walking);
		}
	}
}