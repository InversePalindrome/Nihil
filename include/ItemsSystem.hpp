/*
Copyright (c) 2017 InversePalindrome
Nihil - ItemsSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"

#include <Thor/Time/CallbackTimer.hpp>

#include <vector>
#include <unordered_map>
#include <functional>


class ItemsSystem : public System
{
public:
	ItemsSystem(Entities& entities, Events& events);

	virtual void update(float deltaTime);

private:
	std::vector<thor::CallbackTimer> powerUpTimers;
	std::unordered_map<Item, std::function<void(PhysicsComponent&, PowerUpComponent&)>> powerUpEffects;

	void handleItemPickup(Entity collector, Entity item);
};