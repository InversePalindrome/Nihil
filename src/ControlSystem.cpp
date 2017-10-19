/*
Copyright (c) 2017 InversePalindrome
Nihil - ControlSystem.cpp
InversePalindrome.com
*/


#include "ControlSystem.hpp"


ControlSystem::ControlSystem(Entities& entities, Events& events, sf::RenderWindow& window) :
	System(entities, events),
	window(window)
{
	events.subscribe<IsMidAir>([this](const auto& event) { setMidAirStatus(event.entity, event.midAirStatus); });
	events.subscribe<CanConversate>([this](const auto& event) { setConversationStatus(event.entity, event.conversatingEntity, event.conversationStatus); });

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
	if (entity.has_component<ControllableComponent>())
	{
		this->inputHandler.clearCallbacks();

		this->inputHandler.addCallback("Move Left", [this, entity]() mutable
		{
			entity.sync();

			this->events.broadcast(DirectionChanged{ entity, Direction::Left });
		});

		this->inputHandler.addCallback("Move Right", [this, entity]() mutable
		{
			entity.sync();

			this->events.broadcast(DirectionChanged{ entity, Direction::Right });
		});

		this->inputHandler.addCallback("Jump", [this, entity]() mutable
		{
			entity.sync();
			
			if (!entity.get_component<ControllableComponent>().isMidAir())
			{
				this->events.broadcast(Jumped{ entity });
			}
		});

		this->inputHandler.addCallback("Shoot", [this, entity]() mutable
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

		this->inputHandler.addCallback("Conversate", [this, entity]() mutable
		{
			entity.sync();

			this->entities.for_each<DialogComponent>([this, entity](auto dialogEntity, auto& dialog)
			{
				if (entity.get_component<ControllableComponent>().getCurrentConversationID() == dialog.getConversationID())
				{
					this->events.broadcast(DisplayConversation{ dialogEntity, true });
					this->events.broadcast(UpdateConversation{ dialogEntity });
				}
			});
		});
	}
}

void ControlSystem::update(float deltaTime)
{
	this->inputHandler.clearEvents();

	this->inputHandler.update(this->window);
	this->inputHandler.invokeCallbacks(this->window);

	this->callbacks.update();
	this->callbacks.clear();
}

void ControlSystem::setMidAirStatus(Entity entity, bool midAirStatus)
{
	if (entity.has_component<ControllableComponent>())
	{
		entity.get_component<ControllableComponent>().setMidAirStatus(midAirStatus);
	}
}

void ControlSystem::setConversationStatus(Entity entity, Entity conversatingEntity, bool conversationStatus)
{
	if (entity.has_component<ControllableComponent>() && conversatingEntity.has_component<DialogComponent>())
	{
		auto& controllable = entity.get_component<ControllableComponent>();
		auto& dialog = conversatingEntity.get_component<DialogComponent>();

		controllable.setConversationStatus(conversationStatus);
		controllable.setCurrentConversationID(dialog.getConversationID());
	}
}