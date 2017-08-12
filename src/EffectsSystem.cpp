/*
Copyright (c) 2017 InversePalindrome
Nihil - EffectsSystem.cpp
InversePalindrome.com
*/


#include "EffectsSystem.hpp"


EffectsSystem::EffectsSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	events.subscribe<StateChanged>([this](const auto& event) { changeParticleEffect(event.entity, event.state); });
}

void EffectsSystem::update(float deltaTime)
{
	entities.for_each<ParticleComponent, PositionComponent, StateComponent>(
		[this, deltaTime](auto entity, auto& particle, const auto& position, const auto& state)
	{
		particle.setEmitterPosition(position.getPosition() + particle.getEmissionOffset());

		if (particle.hasParticleData(state.getState()))
		{
			particle.update(deltaTime);
		}
	});
}

void EffectsSystem::changeParticleEffect(Entity entity, EntityState state)
{
	if (entity.has_component<ParticleComponent>())
	{
		auto& particle = entity.get_component<ParticleComponent>();

		if (particle.hasParticleData(state))
		{
			particle.setEffect(state);
		}
		else
		{
			particle.clearEffects();
			particle.clearParticles();
		}
	}
}