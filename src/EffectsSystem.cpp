/*
Copyright (c) 2017 InversePalindrome
Nihil - EffectsSystem.cpp
InversePalindrome.com
*/


#include "EffectsSystem.hpp"


EffectsSystem::EffectsSystem(Entities& entities, Events& events) :
	System(entities, events)
{
}

void EffectsSystem::update(float deltaTime)
{
	entities.for_each<ParticleComponent, PositionComponent>(
		[this, deltaTime](auto entity, auto& particle, const auto& position)
	{
		particle.update(deltaTime);
	});
}

