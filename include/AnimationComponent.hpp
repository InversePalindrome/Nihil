/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Direction.hpp"
#include "StateComponent.hpp"

#include <Thor/Animations/Animator.hpp>
#include <Thor/Animations/FrameAnimation.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <map>
#include <optional>


class AnimationComponent
{
	using Animation = std::pair<EntityState, Direction>;

public:
	AnimationComponent(const std::string& animationsFile);

	std::optional<std::pair<EntityState, Direction>> getCurrentAnimation() const;

	void animate(sf::Sprite& sprite, float deltaTime);

	void playAnimation(EntityState state, Direction direction, bool loop);

	bool isPlayingAnimation() const;
	bool hasAnimation(const Animation& animation) const;

private:
	std::map<Animation, std::pair<thor::FrameAnimation, float>> animations;
	thor::Animator<sf::Sprite, std::pair<EntityState, Direction>> animator;
};