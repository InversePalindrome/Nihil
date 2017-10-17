/*
Copyright (c) 2017 InversePalindrome
Nihil - ItemsSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "Callbacks.hpp"

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
	std::unordered_map<Item, std::string> itemNames;
	std::vector<thor::CallbackTimer> powerUpTimers;
	std::unordered_map<Item, std::function<void(Entity, PowerUpComponent&)>> powerUpEffects;

	Callbacks callbacks;

	void handleItemPickup(Entity collector, Entity item);
	void handleItemDrop(Entity dropper);
	void handleKeyPickup(const KeyComponent& key);
};