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
	events.subscribe<SetMidAirStatus>([this](const auto& event) { setMidAirStatus(event.entity, event.midAirStatus); });

	events.subscribe<entityplus::component_added<Entity, ControllableComponent>>([&events](const auto& event)
	{
		if (event.entity.has_component<HealthComponent>())
		{
			events.broadcast(DisplayHealthBar{ event.entity.get_component<HealthComponent>() });
		}
	});
}

void ControlSystem::addControl(Entity entity)
{
	this->inputHandler.clearCallbacks();

	this->inputHandler.addCallback(Action::MoveLeft, [this, entity]() mutable
	{
		entity.sync();

		this->events.broadcast(DirectionChanged{ entity, Direction::Left });
	});

	this->inputHandler.addCallback(Action::MoveRight, [this, entity]() mutable
	{
		entity.sync();

		this->events.broadcast(DirectionChanged{ entity, Direction::Right });
	});

	this->inputHandler.addCallback(Action::Jump, [this, entity]() mutable
	{
		entity.sync();

		if (!entity.get_component<ControllableComponent>().isMidAir())
		{
			this->events.broadcast(Jumped{ entity });
		}
	});

	this->inputHandler.addCallback(Action::Shoot, [this, entity]() mutable
	{
		entity.sync();

		if (entity.has_component<RangeAttackComponent>() && entity.has_component<TimerComponent>()
			&& entity.get_component<TimerComponent>().hasTimerExpired("Reload"))
		{
			this->callbacks.addCallback([this, entity]()
			{
				this->events.broadcast(ShootProjectile{ entity, entity.get_component<RangeAttackComponent>().getProjectileID(),{ 0.f, 0.f } });
			});

			entity.get_component<TimerComponent>().restartTimer("Reload");
		}
	});
}

void ControlSystem::update(float deltaTime)
{
	this->callbacks.update();
	this->callbacks.clearCallbacks();
}

void ControlSystem::setMidAirStatus(Entity entity, bool midAirStatus)
{
	if (entity.has_component<ControllableComponent>())
	{
		entity.get_component<ControllableComponent>().setMidAirStatus(midAirStatus);
	}
}