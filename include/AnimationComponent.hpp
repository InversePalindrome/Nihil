/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Direction.hpp"
#include "StateComponent.hpp"

#include <Thor/Animations/Animator.hpp>

#include <SFML/Graphics/Sprite.hpp>


class AnimationComponent
{
public:
	AnimationComponent(const std::string& animationsFile);

	std::pair<EntityState, Direction> getCurrentAnimation() const;

	void animate(sf::Sprite& sprite, float deltaTime);

	void playAnimation(EntityState state, Direction direction, bool loop);

	bool isPlayingAnimation() const;

private:
	thor::Animator<sf::Sprite, std::pair<EntityState, Direction>> animator;
};