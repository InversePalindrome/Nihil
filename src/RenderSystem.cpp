/*
Copyright (c) 2017 InversePalindrome
Nihil - RenderSystem.cpp
InversePalindrome.com
*/


#include "RenderSystem.hpp"
#include "UnitConverter.hpp"
#include "SpriteComponent.hpp"
#include "PositionComponent.hpp"

#include <algorithm>


RenderSystem::RenderSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	events.subscribe<entityplus::component_added<Entity, ChildComponent>>([this](const auto& event) { setParentTransforms(event.entity, event.component); });
	events.subscribe<CreateTransform>([this](const auto& event) 
	{
		addNewTransform(event.child, event.parent);
		setParentTransforms(event.childEntity, event.child);
	});
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
		if (this->isInsideView(target.getView(), sprite))
		{
			target.draw(sprite);
		}
		else if (entity.has_component<ChildComponent>())
		{
			states.transform *= entity.get_component<ChildComponent>().getTransform();

			target.draw(sprite, states);
		}
	});
	this->entities.for_each<ParticleComponent>([&target, states](auto entity, auto& particle)
	{
		target.draw(particle, states); 
	});
}

void RenderSystem::setParentTransforms(Entity childEntity, ChildComponent& child)
{
	auto& parents = entities.get_entities<ParentComponent>();
	
	auto foundEntity = std::find_if(std::begin(parents), std::end(parents), [&child](auto& parentEntity) { return child.getParentID() != -1 && child.getParentID() == parentEntity.get_component<ParentComponent>().getChildID(); });
	
	if (foundEntity != std::end(parents))
	{
		child.setTransform(foundEntity->get_component<SpriteComponent>().getTransform());
		this->transformationIDs.emplace(child.getParentID());

		if (foundEntity->has_component<PositionComponent>())
		{
			const auto& parentPosition = foundEntity->get_component<PositionComponent>().getPosition();

			if (childEntity.has_component<PositionComponent>())
			{
				childEntity.get_component<PositionComponent>().setPosition(foundEntity->get_component<PositionComponent>().getPosition());
			}
			if (childEntity.has_component<PhysicsComponent>())
			{
				childEntity.get_component<PhysicsComponent>().setPosition(
					b2Vec2(UnitConverter::pixelsToMeters(parentPosition.x + 0.1f), UnitConverter::pixelsToMeters(-parentPosition.y)));
			}
		}
	}
}

void RenderSystem::addNewTransform(ChildComponent& child, ParentComponent& parent)
{
	if (parent.getChildID() < 0)
	{
		parent.setChildID(this->getNewTransformationID());
		child.setParentID(this->getNewTransformationID());
	}
	else
	{
		child.setParentID(parent.getChildID());
	}
}

std::int32_t RenderSystem::getNewTransformationID() const
{
	if (!this->transformationIDs.empty())
	{
		return *(std::rbegin(this->transformationIDs)) + 1;
	}
	else
	{
		return 0;
	}
}

bool RenderSystem::isInsideView(const sf::View& view, const SpriteComponent& sprite) const
{
	const auto& left = view.getCenter().x - view.getSize().x / 2.f;
	const auto& right = view.getCenter().x + view.getSize().x / 2.f;
	const auto& top = view.getCenter().y - view.getSize().y / 2.f;
	const auto& bottom = view.getCenter().y + view.getSize().y / 2.f;
	 
	return sprite.getPosition().x > left && sprite.getPosition().x < right
		   && sprite.getPosition().y > top && sprite.getPosition().y < bottom;
}