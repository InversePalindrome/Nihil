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


	events.subscribe<entityplus::component_added<Entity, RangeAttackComponent>>([this](const auto& event) { addReloadTimer(event.entity, event.component); });
	events.subscribe<CombatOcurred>([this](const auto& event) { handleCombat(event.attacker, event.victim); });
	events.subscribe<ShootProjectile>([this](const auto& event) { shootProjectile(); });
}

void CombatSystem::update(float deltaTime)
{
	
}

void CombatSystem::handleCombat(Entity attacker, Entity victim)
{
	const auto& damagePoints = attacker.get_component<MeleeAttackComponent>().getDamagePoints();
	auto& health = victim.get_component<HealthComponent>();

	if (health.getHitpoints() > 0u)
	{
		health.setHitpoints(health.getHitpoints() - damagePoints);
	}
	if (victim.has_component<ControllableComponent>())
	{
		this->events.broadcast(DisplayHealthBar{ victim });
	}
	if (health.getHitpoints() == 0u)
	{
		this->events.broadcast(ChangeState{ victim, EntityState::Dead });
	}
}

void CombatSystem::shootProjectile()
{
	
}

void CombatSystem::addReloadTimer(Entity entity, RangeAttackComponent& rangeAttack)
{
	if (entity.has_component<TimerComponent>())
	{
		auto& timer = entity.get_component<TimerComponent>();

		timer.addTimer("Reload", rangeAttack.getReloadTime());
		timer.restartTimer("Reload");
	}
}