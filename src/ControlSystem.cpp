/*
Copyright (c) 2017 InversePalindrome
Nihil - ControlSystem.cpp
InversePalindrome.com
*/


#include "ControlSystem.hpp"


ControlSystem::ControlSystem(Entities& entities, Events& events, InputHandler& inputHandler) :
	System(entities, events),
	inputHandler(inputHandler)
{
	events.subscribe<entityplus::component_added<Entity, ControllableComponent>>([&events](const auto& event)
	{
		if (event.entity.has_component<HealthComponent>())
		{
			events.broadcast(DisplayHealthBar{ event.entity });
		}
	});
}

void ControlSystem::update(float deltaTime)
{
	this->entities.for_each<ControllableComponent, TimerComponent>(
		[this, deltaTime](auto entity, auto& controllable, auto& timer)
	{
		this->reactToInput(entity, timer);
		this->events.broadcast(MoveCamera{ entity });
	});
}

void ControlSystem::reactToInput(Entity entity, TimerComponent& timer)
{
	if (this->inputHandler.isActive("Move Left"))
	{
		this->events.broadcast(DirectionChanged{ entity, Direction::Left });
	}
	else if (this->inputHandler.isActive("Move Right"))
	{
		this->events.broadcast(DirectionChanged{ entity, Direction::Right });
	}
	else if (this->inputHandler.isActive("Shoot") && entity.has_component<RangeAttackComponent>() && timer.hasTimer("Reload") && timer.hasTimerExpired("Reload"))
	{
		this->events.broadcast(ShootProjectile{ entity, entity.get_component<RangeAttackComponent>().getProjectileID(), {} });
		
		timer.restartTimer("Reload");
	}
	else if (this->inputHandler.isActive("Jump") && timer.hasTimer("JumpInterval") && timer.hasTimerExpired("JumpInterval"))
	{
		this->events.broadcast(Jumped{ entity });
		timer.restartTimer("JumpInterval");
	}
}