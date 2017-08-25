/*
Copyright (c) 2017 InversePalindrome
Nihil - RenderSystem.cpp
InversePalindrome.com
*/


#include "RenderSystem.hpp"
#include "SpriteComponent.hpp"
#include "PositionComponent.hpp"

#include <algorithm>


RenderSystem::RenderSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	events.subscribe<entityplus::component_added<Entity, ChildComponent>>([this](const auto& event) { setParentTransforms(event.entity, event.component); });
}

void RenderSystem::update(float deltaTime)
{
	this->entities.for_each<SpriteComponent, PositionComponent>(
		[](auto entity, auto& sprite, auto& position)
	{
		sprite.setPosition(position.getPosition());
	});
}

void RenderSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	this->entities.for_each<SpriteComponent>([this, &target, states](auto entity, auto& sprite) mutable
	{ 
		if (entity.has_component<ChildComponent>())
		{
			states.transform *= entity.get_component<ChildComponent>().getTransform();
			
			target.draw(sprite, states);
		}
		else
		{
			target.draw(sprite);
		}
	});
	entities.for_each<ParticleComponent>([&target, states](auto entity, auto& particle)
	{
		target.draw(particle, states); 
	});
}

void RenderSystem::setParentTransforms(Entity entity, ChildComponent& child)
{
	auto& entityList = entities.get_entities<ParentComponent>();
	
	auto foundEntity = std::find_if(std::begin(entityList), std::end(entityList), [&child](auto& parentEntity) { return child.getParentID() == parentEntity.get_component<ParentComponent>().getID(); });

	if (foundEntity != std::end(entityList))
	{
		child.setTransform(foundEntity->get_component<SpriteComponent>().getTransform());
	}
}