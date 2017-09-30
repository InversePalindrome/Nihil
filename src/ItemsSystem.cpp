/*
Copyright (c) 2017 InversePalindrome
Nihil - ItemsSystem.cpp
InversePalindrome.com
*/


#include "ItemsSystem.hpp"


ItemsSystem::ItemsSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	events.subscribe<PickedUpItem>([this](const auto& event) { handleItemPickup(event.collector, event.item); });
}

void ItemsSystem::update(float deltaTime)
{

}

void ItemsSystem::handleItemPickup(Entity collector, Entity item)
{
	if (collector.has_component<InventoryComponent>() && item.has_component<PickupComponent>())
	{
		auto& inventory = collector.get_component<InventoryComponent>();
		const auto& pickup = item.get_component<PickupComponent>();

		if (inventory.hasItem(pickup.getItem()))
		{
			inventory[pickup.getItem()]++;
		}
		else
		{
			inventory.addItem(pickup.getItem(), 1u);
		}

		this->events.broadcast(EmitSound{ pickup.getSoundID(), false });
	}

	this->events.broadcast(DestroyEntity{ item });
}