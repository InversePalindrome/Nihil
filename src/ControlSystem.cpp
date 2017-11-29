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
	events.subscribe<entityplus::component_added<Entity, ControllableComponent>>([this, &events](const auto& event)
	{
		if (event.entity.has_component<HealthComponent>())
		{
			events.broadcast(DisplayHealthBar{ event.entity.get_component<HealthComponent>() });
		}

		addControl(event.entity);
	});
}

void ControlSystem::addControl(Entity entity)
{
	this->player = entity;
	this->inputHandler.clearCallbacks();
	
	this->inputHandler.addCallback(Action::MoveLeft, [this, entity]() mutable
	{
		if (entity.sync())
		{
			this->events.broadcast(DirectionChanged{ entity, Direction::Left });
		}
	});

	this->inputHandler.addCallback(Action::MoveRight, [this, entity]() mutable
	{
		if (entity.sync())
		{
			this->events.broadcast(DirectionChanged{ entity, Direction::Right });
		}
	});

	this->inputHandler.addCallback(Action::MoveDown, [this, entity]() mutable
	{
		if (entity.sync() && entity.has_component<StateComponent>() && entity.get_component<StateComponent>().getState() == EntityState::Swimming)
		{
			this->events.broadcast(DirectionChanged{ entity, Direction::Down });
		}
	});

	this->inputHandler.addCallback(Action::MoveUp, [this, entity]() mutable
	{
		if (entity.sync() && entity.has_component<StateComponent>() && entity.get_component<StateComponent>().getState() == EntityState::Swimming)
		{
			this->events.broadcast(DirectionChanged{ entity, Direction::Up });
		}
	});

	this->inputHandler.addCallback(Action::Jump, [this, entity]() mutable
	{
		if (entity.sync())
		{
			this->events.broadcast(Jumped{ entity });
		}
	});

	this->inputHandler.addCallback(Action::Shoot, [this, entity]() mutable
	{
		if (entity.sync() && entity.has_component<RangeAttackComponent>() && entity.has_component<TimerComponent>()
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