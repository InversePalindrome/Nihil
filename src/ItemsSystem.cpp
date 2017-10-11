/*
Copyright (c) 2017 InversePalindrome
Nihil - ItemsSystem.cpp
InversePalindrome.com
*/


#include "ItemsSystem.hpp"


ItemsSystem::ItemsSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	powerUpEffects[Item::SpeedBoost] = [this, &events](auto& physics, auto& powerUp)
	{ 
		const float maxVelocity = physics.getMaxVelocity();

		physics.setMaxVelocity(maxVelocity * (1.f + powerUp.getEffectBoost()));

		powerUpTimers.push_back(thor::CallbackTimer());

		powerUpTimers.back().connect0([maxVelocity, &events, &physics, &powerUp]() 
		{
			physics.setMaxVelocity(maxVelocity);

			events.broadcast(HidePowerUp{ powerUp.getItem() });
		});

		powerUpTimers.back().restart(sf::seconds(powerUp.getEffectTime()));

		events.broadcast(DisplayPowerUp{ powerUp.getItem() });
	};

	powerUpEffects[Item::JumpBoost] = [this, &events](auto& physics, auto& powerUp)
	{ 
		const float jumpVelocity = physics.getJumpVelocity();

		physics.setJumpVelocity(jumpVelocity * (1.f + powerUp.getEffectBoost()));

		powerUpTimers.push_back(thor::CallbackTimer());

		powerUpTimers.back().connect0([jumpVelocity, &events, &physics, &powerUp]() 
		{
			physics.setJumpVelocity(jumpVelocity);

			events.broadcast(HidePowerUp{ powerUp.getItem() });
		});

		powerUpTimers.back().restart(sf::seconds(powerUp.getEffectTime()));

		events.broadcast(DisplayPowerUp{ powerUp.getItem() });
	};

	events.subscribe<PickedUpItem>([this](const auto& event) { handleItemPickup(event.collector, event.item); });
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
			this->powerUpEffects[powerUp.getItem()](collector.get_component<PhysicsComponent>(), powerUp);
		}
	}

	this->events.broadcast(DestroyEntity{ item });
}