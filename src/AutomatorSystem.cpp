/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatorSystem.cpp
InversePalindrome.com
*/


#include "AutomatorSystem.hpp"


AutomatorSystem::AutomatorSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	registeredTasks["Left"] = [&events](auto entity) { events.broadcast(SetVelocity{ entity, Direction::Left}); };
	registeredTasks["Right"] = [&events](auto entity) { events.broadcast(SetVelocity{ entity, Direction::Right }); };
	registeredTasks["Up"] = [&events](auto entity) { events.broadcast(SetVelocity{ entity, Direction::Up }); };
	registeredTasks["Down"] = [&events](auto entity) { events.broadcast(SetVelocity{ entity, Direction::Down }); };
	registeredTasks["LeftUp"] = [&events](auto entity) { events.broadcast(SetVelocity{ entity, Direction::LeftUp }); };
	registeredTasks["LeftDown"] = [&events](auto entity) { events.broadcast(SetVelocity{ entity, Direction::LeftDown }); };
	registeredTasks["RightUp"] = [&events](auto entity) { events.broadcast(SetVelocity{ entity, Direction::RightUp }); };
    registeredTasks["RightDown"] = [&events](auto entity) { events.broadcast(SetVelocity{ entity, Direction::RightDown }); };
	registeredTasks["Wait"] = [&events](auto entity) { events.broadcast(StopMovement{ entity }); };

	events.subscribe<SetAutomatedStatus>([this](const auto& event) { setActiveStatus(event.entity, event.activeStatus); });
}

void AutomatorSystem::update(float deltaTime)
{
	this->entities.for_each<AutomatedComponent>([this, deltaTime](auto entity, auto& automated)
	{
		automated.update(deltaTime);
		automated.playCurrentTask();

		if (automated.isActive() && automated.hasTasks())
		{
			if (automated.hasCurrentTaskExpired())
			{
				automated.pushNextTask();
			}

			this->sendTask(entity, automated);
		}
	});
}

void AutomatorSystem::sendTask(Entity entity, AutomatedComponent& automated)
{
	this->registeredTasks[automated.getCurrentTask().first](entity);
}

void AutomatorSystem::setActiveStatus(Entity entity, bool activeStatus)
{
	if (entity.has_component<AutomatedComponent>())
	{
		entity.get_component<AutomatedComponent>().setActiveStatus(activeStatus); 
	}
}