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
	events.subscribe<IsMidAir>([this](const auto& event) { setMidAirStatus(event.entity, event.midAirStatus); });

	events.subscribe<entityplus::component_added<Entity, ControllableComponent>>([&events](const auto& event)
	{
		if (event.entity.has_component<HealthComponent>())
		{
			events.broadcast(DisplayHealthBar{ event.entity.get_component<HealthComponent>() });
		}
	});
}

void ControlSystem::update(float deltaTime)
{
	this->entities.for_each<ControllableComponent, PositionComponent, TimerComponent>(
		[this, deltaTime](auto entity, auto& controllable, auto& position, auto& timer)
	{
		this->reactToInput(entity, controllable, timer);
	});

	callbacks.update();
	callbacks.clear();
}

void ControlSystem::reactToInput(Entity entity, ControllableComponent& controllable, TimerComponent& timer)
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
		this->callbacks.addCallback([this, entity]()
		{
			this->events.broadcast(ShootProjectile{ entity, entity.get_component<RangeAttackComponent>().getProjectileID(), {0.f, 0.f} });
		});
		
		timer.restartTimer("Reload");
	}
	else if (this->inputHandler.isActive("Jump") && !controllable.isMidAir())
	{
		this->events.broadcast(Jumped{ entity });
	}
}


void ControlSystem::setMidAirStatus(Entity entity, bool midAirStatus)
{
	if (entity.has_component<ControllableComponent>())
	{
		entity.get_component<ControllableComponent>().setMidAirStatus(midAirStatus);
	}
}