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
}

void StateSystem::update(float deltaTime)
{
	
}

void StateSystem::changeState(Entity entity, EntityState state)
{
	auto& stateComponent = entity.get_component<StateComponent>();
	
	if (stateComponent.getState() != state)
	{
		stateComponent.setState(state);
		this->events.broadcast(StateChanged{ entity, state });
	}
}