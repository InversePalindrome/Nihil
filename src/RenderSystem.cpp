/*
Copyright (c) 2017 InversePalindrome
Nihil - RenderSystem.cpp
InversePalindrome.com
*/


#include "RenderSystem.hpp"
#include "UnitConverter.hpp"
#include "ViewUtility.hpp"
#include "EntityUtility.hpp"
#include "SpriteComponent.hpp"
#include "PositionComponent.hpp"

#include <brigand/algorithms/for_each.hpp>

#include <algorithm>


RenderSystem::RenderSystem(Entities& entities, Events& events) :
    System(entities, events)
{
    events.subscribe<CreateTransform>([this](const auto & event)
        {
            setParentTransforms(event.childEntity, event.parentEntity, event.offset);
        });
}

void RenderSystem::update(float deltaTime)
{
    brigand::for_each<Renderables>([this](auto renderableComponent)
        {
            using Type = decltype(renderableComponent)::type;

            this->entities.for_each<Type, PositionComponent>(
                [](auto entity, auto & renderable, auto & position)
                {
                    renderable.setPosition(position.getPosition() + renderable.getOffset());
                });
        });
}

void RenderSystem::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    brigand::for_each<Renderables>([this, &target, states](auto renderableComponent)
        {
            using Type = decltype(renderableComponent)::type;

            this->entities.for_each<Type>([this, &target, states](auto entity, const auto & renderable) mutable
                {
                    if (Utility::isInsideView(target.getView(), renderable.getPosition(), renderable.getGlobalBounds()))
                    {
                        target.draw(renderable, states);
                    }
                });
        });
}

void RenderSystem::setParentTransforms(Entity childEntity, Entity parentEntity, const sf::Vector2f & offset)
{
    if (childEntity.has_component<ChildComponent>() && parentEntity.has_component<ParentComponent>()
        && parentEntity.has_component<SpriteComponent>() && parentEntity.has_component<PositionComponent>())
    {
        auto& child = childEntity.get_component<ChildComponent>();
        auto& parent = parentEntity.get_component<ParentComponent>();

        child.setParentID(parent.getEntityID());
        parent.setChildID(child.getEntityID());

        child.setTransform(parentEntity.get_component<SpriteComponent>().getTransform());

        Utility::setPosition(childEntity, parentEntity.get_component<PositionComponent>().getPosition() + offset);
    }
}