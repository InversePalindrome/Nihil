/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimatorSystem.cpp
InversePalindrome.com
*/


#include "AnimatorSystem.hpp"
#include "SpriteComponent.hpp"
#include "AnimationComponent.hpp"


AnimatorSystem::AnimatorSystem(Entities& entities, Events& events) :
	System(entities, events)
{
	events.subscribe<StateChanged>([this](const auto& event) { changeAnimationState(event.entity, event.state); });
	events.subscribe<DirectionChanged>([this](const auto& event) { changeAnimationDirection(event.entity, event.direction); });
}

void AnimatorSystem::update(float deltaTime)
{
	this->entities.for_each<AnimationComponent, SpriteComponent>(
    [deltaTime](auto entity, auto& animation, auto& sprite)
	{
		if (animation.isPlayingAnimation())
		{
			animation.animate(sprite.getSprite(), deltaTime);
		}
	});
}

void AnimatorSystem::changeAnimationState(Entity entity, EntityState state)
{
	if (entity.has_component<AnimationComponent>())
	{
		auto& animation = entity.get_component<AnimationComponent>();

		const auto& currentAnimation = animation.getCurrentAnimation();

		if (currentAnimation.has_value() && animation.hasAnimation(std::make_pair(state, currentAnimation.value().second)) &&
			currentAnimation.value().first != state)
		{
			animation.playAnimation(state, currentAnimation.value().second, true);
		}
	}
}

void AnimatorSystem::changeAnimationDirection(Entity entity, Direction direction)
{
	if (entity.has_component<AnimationComponent>())
	{
		auto& animation = entity.get_component<AnimationComponent>();

		const auto& currentAnimation = animation.getCurrentAnimation();

		if (currentAnimation.has_value() && animation.hasAnimation(std::make_pair(currentAnimation.value().first, direction)) && 
			currentAnimation.value().second != direction)
		{
			animation.playAnimation(currentAnimation.value().first, direction, true);
		}
	}
}