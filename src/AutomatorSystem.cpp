/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatorSystem.cpp
InversePalindrome.com
*/


#include "AutomatorSystem.hpp"
#include "CollisionData.hpp"


AutomatorSystem::AutomatorSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	events.subscribe<AddedUserData>([this](auto& event) { addTasks(event.entity); });

	for (std::size_t i = 0u; i < static_cast<std::size_t>(Direction::Size); ++i)
	{
		auto direction = static_cast<Direction>(i);

		registeredTasks[direction] = [&events, direction](auto entity) { events.broadcast(SetVelocity{ entity, direction }); };
	}
}

void AutomatorSystem::update(float deltaTime)
{
	this->entities.for_each<AutomatedComponent>([this, deltaTime](auto entity, auto& automated)
	{
		if (automated.hasTasks())
		{
			if (this->hasCompletedTask(entity, automated.getCurrentTask().second))
			{
				automated.pushNextTask();
			}
			
			this->sendTask(entity, automated);
		}
	});
}

void AutomatorSystem::addTasks(Entity entity)
{
	if (entity.has_component<AutomatedComponent>() && entity.has_component<PhysicsComponent>())
	{
		auto& automated = entity.get_component<AutomatedComponent>();
		auto taskFile = static_cast<CollisionData*>(entity.get_component<PhysicsComponent>().getUserData(ObjectType::Platform))->properties["TaskFile"].getStringValue();

		entity.get_component<AutomatedComponent>().loadTasks(taskFile);
	}
}

void AutomatorSystem::sendTask(Entity entity, AutomatedComponent& automated)
{
	this->registeredTasks[automated.getCurrentTask().first](entity);
}

bool AutomatorSystem::hasCompletedTask(Entity entity, const b2Vec2& target) const
{
	if (entity.has_component<PhysicsComponent>() && entity.get_component<PhysicsComponent>().isIntersecting(target))
	{
		return true;
	}

	return false;
}