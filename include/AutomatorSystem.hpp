/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatorSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"

#include <functional>
#include <unordered_map>


class AutomatorSystem : public System
{
public:
	AutomatorSystem(Entities& entities, Events& events);

	virtual void update(float deltaTime) override;

private:
	std::unordered_map<Direction, std::function<void(Entity)>> registeredTasks;

	void addTasks(Entity entity);

	void sendTask(Entity entity, AutomatedComponent& automated);

	bool hasCompletedTask(Entity entity, const b2Vec2& target) const;
};