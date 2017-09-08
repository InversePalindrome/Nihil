/*
Copyright (c) 2017 InversePalindrome
Nihil - CombatSystem.cpp
InversePalindrome.com
*/


#include "CombatSystem.hpp"


CombatSystem::CombatSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	events.subscribe<entityplus::component_added<Entity, HealthComponent>>([&events](const auto& event)
	{
		if (event.entity.has_component<ControllableComponent>())
		{
			events.broadcast(DisplayHealthBar{ event.entity });
		}
	});

	events.subscribe<CombatOcurred>([this](const auto& event) { handleCombat(event.attacker, event.victim); });
}

void CombatSystem::update(float deltaTime)
{
	this->entities.for_each<HealthComponent>(
		[this](auto entity, auto& health)
	{
		this->checkIfDead(entity, health);
	});
}

void CombatSystem::handleCombat(Entity attacker, Entity victim)
{
	const auto& damagePoints = attacker.get_component<AttackComponent>().getDamagePoints();
	auto& health = victim.get_component<HealthComponent>();

	if (health.getHitpoints() > 0u)
	{
		health.setHitpoints(health.getHitpoints() - damagePoints);
	}
	if (victim.has_component<ControllableComponent>())
	{
		this->events.broadcast(DisplayHealthBar{ victim });
	}
}

void CombatSystem::checkIfDead(Entity entity, HealthComponent& health)
{
	if (health.getHitpoints() == 0u)
	{
		this->events.broadcast(ChangeState{ entity, EntityState::Dead });
	}
}