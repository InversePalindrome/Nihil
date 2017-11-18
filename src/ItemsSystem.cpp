/*
Copyright (c) 2017 InversePalindrome
Nihil - ItemsSystem.cpp
InversePalindrome.com
*/


#include "ItemsSystem.hpp"


ItemsSystem::ItemsSystem(Entities& entities, Events& events) :
	System(entities, events),
	itemNames({ {Item::SpeedBoost, "SpeedBoost.txt"}, {Item::JumpBoost, "JumpBoost.txt"}, {Item::Laser, "LaserBoost.txt"}, {Item::Heart, "Heart.txt" } })
{
	powerUpEffects[Item::SpeedBoost] = [this, &events](auto collector, auto& powerUp)
	{ 
		const auto maxVelocity = collector.get_component<PhysicsComponent>().getMaxVelocity();
		
		collector.get_component<PhysicsComponent>().setMaxVelocity({ maxVelocity.x * (1.f + powerUp.getEffectBoost()), maxVelocity.y * (1.f + powerUp.getEffectBoost()) });

		powerUpTimers.push_back(thor::CallbackTimer());

		powerUpTimers.back().connect0([maxVelocity, collector, powerUp, &events]() mutable 
		{
			if (collector.sync())
			{
				collector.get_component<PhysicsComponent>().setMaxVelocity(maxVelocity);
				collector.get_component<InventoryComponent>().removeItem(powerUp.getItem());

				events.broadcast(HidePowerUp{ powerUp.getItem() });
			}
		});

		powerUpTimers.back().restart(sf::seconds(powerUp.getEffectTime()));
		
		events.broadcast(DisplayPowerUp{ powerUp.getItem() });
	};

	powerUpEffects[Item::JumpBoost] = [this, &events](auto collector, auto& powerUp)
	{ 
		const auto jumpVelocity = collector.get_component<PhysicsComponent>().getJumpVelocity();

		collector.get_component<PhysicsComponent>().setJumpVelocity(jumpVelocity * (1.f + powerUp.getEffectBoost()));

		powerUpTimers.push_back(thor::CallbackTimer());

		powerUpTimers.back().connect0([jumpVelocity, collector, powerUp, &events]() mutable
		{
			if (collector.sync())
			{
				collector.get_component<PhysicsComponent>().setJumpVelocity(jumpVelocity);
				collector.get_component<InventoryComponent>().removeItem(powerUp.getItem());

				events.broadcast(HidePowerUp{ powerUp.getItem() });
			}
		});
	
		powerUpTimers.back().restart(sf::seconds(powerUp.getEffectTime()));

		events.broadcast(DisplayPowerUp{ powerUp.getItem() });
	};

	powerUpEffects[Item::Laser] = [this, &events](auto collector, auto& powerUp)
	{
		collector.add_component<RangeAttackComponent>("Laser", powerUp.getEffectBoost());
	
		powerUpTimers.push_back(thor::CallbackTimer());

		powerUpTimers.back().connect0([collector, powerUp, &events]() mutable
		{
			if (collector.sync())
			{
				collector.remove_component<RangeAttackComponent>();
				collector.get_component<InventoryComponent>().removeItem(powerUp.getItem());

				events.broadcast(HidePowerUp{ powerUp.getItem() });
			}
		});

		powerUpTimers.back().restart(sf::seconds(powerUp.getEffectTime()));

		events.broadcast(DisplayPowerUp{ powerUp.getItem() });
	};

	powerUpEffects[Item::Heart] = [this, &events](auto collector, auto& powerUp)
	{
		if (collector.has_component<HealthComponent>())
		{
			auto& health = collector.get_component<HealthComponent>();

			health.setHitpoints(health.getHitpoints() + 1u);

			collector.get_component<InventoryComponent>().removeItem(powerUp.getItem());

			this->events.broadcast(DisplayHealthBar{ health });
		}
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

	this->callbacks.update();
	this->callbacks.clearCallbacks();
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
			this->events.broadcast(DisplayCoins{});
			this->events.broadcast(UpdateAchievement{ Achievement::Collector });
		}
	}
	else if (collector.has_component<InventoryComponent>() && collector.has_component<PhysicsComponent>() && item.has_component<PowerUpComponent>())
	{
		auto& inventory = collector.get_component<InventoryComponent>();
		auto& powerUp = item.get_component<PowerUpComponent>();
		
		if (this->powerUpEffects.count(powerUp.getItem()))
		{
			if (!inventory.hasItem(powerUp.getItem()))
			{
				this->powerUpEffects[powerUp.getItem()](collector, powerUp);

				inventory.addItem(powerUp.getItem(), 1);
			}

			this->events.broadcast(EmitSound{ powerUp.getSoundID(), false });
		}
	}
	else if (collector.has_component<InventoryComponent>() && item.has_component<KeyComponent>())
	{
		this->handleKeyPickup(item.get_component<KeyComponent>());
	}

	this->events.broadcast(DestroyEntity{ item });
}

void ItemsSystem::handleItemDrop(Entity dropper)
{
	if (dropper.has_component<DropComponent>() && dropper.has_component<PositionComponent>())
	{
		if (auto item = dropper.get_component<DropComponent>().getDrop())
		{
			this->events.broadcast(CreateEntity{ this->itemNames[item.value()],
				dropper.get_component<PositionComponent>().getPosition() });
		}
	}
}

void ItemsSystem::handleKeyPickup(const KeyComponent& key)
{
	this->entities.for_each<LockComponent, PhysicsComponent>([this, &key](auto entity, auto& lock, auto& physics)
	{
		if (key.getKeyID() == lock.getUnlockID())
		{
			if (entity.has_component<SpriteComponent>())
			{
				entity.get_component<SpriteComponent>().setSprite(lock.getNewSpriteFile());
			}

			this->callbacks.addCallback([this, entity]() mutable
			{
				entity.sync();

				this->events.broadcast(DestroyBody{ entity.get_component<PhysicsComponent>() });

				entity.remove_component<LockComponent>();
				entity.remove_component<PhysicsComponent>();
			});
		}
	});
}