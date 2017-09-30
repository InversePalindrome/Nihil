/*
Copyright (c) 2017 InversePalindrome
Nihil - ItemsSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"


class ItemsSystem : public System
{
public:
	ItemsSystem(Entities& entities, Events& events);

	virtual void update(float deltaTime);

private:
	void handleItemPickup(Entity collector, Entity item);
};