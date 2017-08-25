/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatorSystem.cpp
InversePalindrome.com
*/


#include "AutomatorSystem.hpp"


AutomatorSystem::AutomatorSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	registeredTasks["Left"] = [&events](auto entity) { events.broadcast(DirectionChanged{ entity, Direction::Left}); };
	registeredTasks["Right"] = [&events](auto entity) { events.broadcast(DirectionChanged{ entity, Direction::Right }); };
	registeredTasks["Up"] = [&events](auto entity) { events.broadcast(DirectionChanged{ entity, Direction::Up }); };
	registeredTasks["Down"] = [&events](auto entity) { events.broadcast(DirectionChanged{ entity, Direction::Down }); };
}

void AutomatorSystem::update(float deltaTime)
{
	this->entities.for_each<AutomatedComponent>([this](auto entity, auto& automated)
	{
		if (automated.hasTasks())
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