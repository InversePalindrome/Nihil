/*
Copyright (c) 2017 InversePalindrome
Nihil - ItemsSystem.cpp
InversePalindrome.com
*/


#include "ItemsSystem.hpp"


ItemsSystem::ItemsSystem(Entities& entities, Events& events) :
	System(entities, events),
	itemNames({{Item::SpeedBoost, "SpeedBoost.txt"}, {Item::JumpBoost, "JumpBoost.txt"}})
{
	powerUpEffects[Item::SpeedBoost] = [this, &events](auto collector, auto& powerUp)
	{ 
		const float maxVelocity = collector.get_component<PhysicsComponent>().getMaxVelocity();
		
	    collector.get_component<PhysicsComponent>().setMaxVelocity(maxVelocity * (1.f + powerUp.getEffectBoost()));

		powerUpTimers.push_back(thor::CallbackTimer());

		powerUpTimers.back().connect0([maxVelocity, collector, &events, &powerUp]() mutable 
		{
			collector.sync();
			
			collector.get_component<PhysicsComponent>().setMaxVelocity(maxVelocity);
			
			events.broadcast(HidePowerUp{ powerUp.getItem() });
		});

		powerUpTimers.back().restart(sf::seconds(powerUp.getEffectTime()));

		events.broadcast(DisplayPowerUp{ powerUp.getItem() });
	};

	powerUpEffects[Item::JumpBoost] = [this, &events](auto collector, auto& powerUp)
	{ 
		const float jumpVelocity = collector.get_component<PhysicsComponent>().getJumpVelocity();

		collector.get_component<PhysicsComponent>().setJumpVelocity(jumpVelocity * (1.f + powerUp.getEffectBoost()));

		powerUpTimers.push_back(thor::CallbackTimer());

		powerUpTimers.back().connect0([jumpVelocity, collector, &events, &powerUp]() mutable
		{
			collector.sync();

			collector.get_component<PhysicsComponent>().setJumpVelocity(jumpVelocity);
		
			events.broadcast(HidePowerUp{ powerUp.getItem() });
		});
	
		powerUpTimers.back().restart(sf::seconds(powerUp.getEffectTime()));

		events.broadcast(DisplayPowerUp{ powerUp.getItem() });
	};

	events.subscribe<PickedUpItem>([this](const auto& event) { handleItemPickup(event.collector, event.item); });
	events.subscribe<DroppedItem>([this](const auto& event) { handleItemDrop(event.dropper); });
}

void ItemsSystem::update(float deltaTime)
{
	for (auto powerUpTimer = std::begin(this->powerUpTimers); powerUpTimer != std::end(this->powerUpTimers); )
	{
		powerUpTimer->update();

		if (powerUpTimer->isExpired())
		{
			powerUpTimer = this->powerUpTimers.erase(powerUpTimer);
		}
		else
		{
			++powerUpTimer;
		}
	}
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

		if (collector.has_component<ControllableComponent>() && pickup.getItem() == Item::Coin)
		{
			this->events.broadcast(DisplayCoins{ inventory });
		}
	}
	else if (collector.has_component<PhysicsComponent>() && item.has_component<PowerUpComponent>())
	{
		auto& powerUp = item.get_component<PowerUpComponent>();
		
		if (this->powerUpEffects.count(powerUp.getItem()))
		{
			this->powerUpEffects[powerUp.getItem()](collector, powerUp);
			this->events.broadcast(EmitSound{ powerUp.getSoundID(), false });
		}
	}

	this->events.broadcast(DestroyEntity{ item });
}

void ItemsSystem::handleItemDrop(Entity dropper)
{
	if (dropper.has_component<DropComponent>() && dropper.has_component<PositionComponent>())
	{
		const auto& drop = dropper.get_component<DropComponent>();

		if (auto item = dropper.get_component<DropComponent>().getDrop())
		{
			this->events.broadcast(CreateEntity{ this->itemNames[item.value()],
				dropper.get_component<PositionComponent>().getPosition() });
		}
	}
}