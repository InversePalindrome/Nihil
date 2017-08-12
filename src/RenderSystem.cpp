/*
Copyright (c) 2017 InversePalindrome
Nihil - RenderSystem.cpp
InversePalindrome.com
*/


#include "RenderSystem.hpp"
#include "SpriteComponent.hpp"
#include "PositionComponent.hpp"


RenderSystem::RenderSystem(Entities& entities, Events& events) :
	System(entities, events)
{
}

void RenderSystem::update(float deltaTime)
{
	entities.for_each<SpriteComponent, PositionComponent>(
		[](auto entity, auto& sprite, auto& position)
	{
		sprite.setPosition(position.getPosition());
	});
}

void RenderSystem::draw(sf::RenderTarget& target)
{
	entities.for_each<SpriteComponent>([&target](auto entity, auto& sprite) { sprite.draw(target); });
	entities.for_each<ParticleComponent>([&target](auto entity, auto& particle) { particle.draw(target); });
}