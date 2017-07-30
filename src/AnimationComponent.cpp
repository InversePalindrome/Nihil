/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationComponent.cpp
InversePalindrome.com
*/


#include "AnimationComponent.hpp"

#include <Thor/Animations/FrameAnimation.hpp>

#include <tuple>
#include <vector>
#include <fstream>
#include <sstream>


AnimationComponent::AnimationComponent(const std::string& animationsFile)
{
	std::ifstream inFile(animationsFile);
	std::string line;

	std::vector<std::tuple<EntityState, Direction, thor::FrameAnimation, float>> animations;
	
	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string category;

		iStream >> category;

		if (category == "Animation")
		{
			std::size_t state, direction;
			float duration;

			iStream >> state >> direction >> duration;

			animations.emplace_back(static_cast<EntityState>(state), static_cast<Direction>(direction), thor::FrameAnimation(), duration);
		}
		else if (category == "Frame")
		{
			float frameTime;
			std::size_t left, top, width, length;

			iStream >> frameTime >> left >> top >> width >> length;

			std::get<2>(animations.back()).addFrame(frameTime, sf::IntRect(left, top, width, length));
		}
	}

	for (const auto& animation : animations)
	{
		animator.addAnimation(std::make_pair(std::get<0>(animation), std::get<1>(animation)), std::get<2>(animation), sf::seconds(std::get<3>(animation)));
	}
	
	animator.playAnimation(std::make_pair(EntityState::Idle, Direction::Right), true);
}

std::pair<EntityState, Direction> AnimationComponent::getCurrentAnimation() const
{
	return this->animator.getPlayingAnimation();
}

void AnimationComponent::animate(sf::Sprite& sprite, float deltaTime)
{
	this->animator.animate(sprite);
	this->animator.update(sf::seconds(deltaTime));
}

void AnimationComponent::playAnimation(EntityState state, Direction direction, bool loop)
{
	this->animator.playAnimation(std::make_pair(state, direction), loop);
}

bool AnimationComponent::isPlayingAnimation() const
{
	return this->animator.isPlayingAnimation();
}