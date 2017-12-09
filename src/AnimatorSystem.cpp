/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimatorSystem.cpp
InversePalindrome.com
*/


#include "AnimatorSystem.hpp"
#include "SpriteComponent.hpp"
#include "ViewUtility.hpp"
#include "AnimationComponent.hpp"


AnimatorSystem::AnimatorSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	events.subscribe<entityplus::component_added<Entity, AnimationComponent>>([this](const auto& event)
	{
		playStartingAnimation(event.entity, event.component);
	});
	events.subscribe<PlayAnimation>([this](const auto& event) { playAnimation(event.entity, event.animation, event.loop); });
	events.subscribe<StopAnimation>([this](const auto& event) { stopAnimation(event.entity); });
	events.subscribe<StateChanged>([this](const auto& event) { changeAnimationState(event.entity, event.state); });
	events.subscribe<DirectionChanged>([this](const auto& event) { changeAnimationDirection(event.entity, event.direction); });
}

void AnimatorSystem::update(float deltaTime)
{
	this->entities.for_each<AnimationComponent>(
    [deltaTime](auto entity, auto& animation)
	{
		animation.update(deltaTime);
	});
}

void AnimatorSystem::animate(sf::RenderTarget& target)
{
	this->entities.for_each<AnimationComponent, SpriteComponent>(
		[&target](auto entity, auto& animation, auto& sprite)
	{
		if (Utility::isInsideView(target.getView(), sprite.getPosition(), sprite.getGlobalBounds()) && animation.isPlayingAnimation())
		{
			animation.animate(sprite.getSprite());
		}
	});
}

void AnimatorSystem::playAnimation(Entity entity, const Animation& animation, bool loop)
{
	if (entity.has_component<AnimationComponent>())
	{
		entity.get_component<AnimationComponent>().playAnimation(animation, loop);
	}
}

void AnimatorSystem::stopAnimation(Entity entity)
{
	if (entity.has_component<AnimationComponent>())
	{
		entity.get_component<AnimationComponent>().stopAnimation();
	}
}

void AnimatorSystem::changeAnimationState(Entity entity, EntityState state)
{
	if (entity.has_component<AnimationComponent>() && entity.has_component<PhysicsComponent>())
	{
		auto& animation = entity.get_component<AnimationComponent>();
		
		animation.playAnimation({ state, entity.get_component<PhysicsComponent>().getDirection() }, true);
	}
}

void AnimatorSystem::changeAnimationDirection(Entity entity, Direction direction)
{
	if (entity.has_component<AnimationComponent>() && entity.has_component<StateComponent>())
	{
		auto& animation = entity.get_component<AnimationComponent>();

		animation.playAnimation({ entity.get_component<StateComponent>().getState(), direction }, true);
	}
}

void AnimatorSystem::playStartingAnimation(Entity entity, AnimationComponent& animation)
{
	if (entity.has_component<PhysicsComponent>())
	{
		switch (entity.get_component<PhysicsComponent>().getObjectType())
		{
		case ObjectType::Player:
		case ObjectType::Character:
		case ObjectType::Pickup:
			animation.playAnimation({ EntityState::Idle, Direction::Right }, true);
			break;
		}
	}
}