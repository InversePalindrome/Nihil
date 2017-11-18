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
	std::unordered_map<std::string, std::function<void(Entity)>> registeredTasks;

	void sendTask(Entity entity, AutomatedComponent& automated);

	void setActiveStatus(Entity entity, bool activeStatus);
};