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
    entities.for_each<ParticleComponent>(
        [this, deltaTime](auto entity, auto & particle)
        {
            particle.update(deltaTime);
        });
}

