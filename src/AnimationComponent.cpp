/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationComponent.cpp
InversePalindrome.com
*/


#include "AnimationComponent.hpp"

#include <fstream>
#include <sstream>


AnimationComponent::AnimationComponent(const std::string& animationsFile) 
{
	std::ifstream inFile(animationsFile);
	std::string line;

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

			animations.emplace(std::make_pair(static_cast<EntityState>(state), static_cast<Direction>(direction)), std::make_pair(thor::FrameAnimation(), duration));
		}
		else if (category == "Frame")
		{
			float frameTime;
			std::size_t left, top, width, length;

			iStream >> frameTime >> left >> top >> width >> length;

			animations.rbegin()->second.first.addFrame(frameTime, sf::IntRect(left, top, width, length));
		}
	}

	for (const auto& animation : animations)
	{
		animator.addAnimation(animation.first, animation.second.first, sf::seconds(animation.second.second));
	}
	
	if (!animations.empty())
	{
		animator.playAnimation(animations.begin()->first, true);
	}
}

std::optional<std::pair<EntityState, Direction>> AnimationComponent::getCurrentAnimation() const
{
	if (this->animator.isPlayingAnimation())
	{
		return this->animator.getPlayingAnimation();
	}
	else
	{
		return {};
	}
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

bool AnimationComponent::hasAnimation(const Animation& animation) const
{
	return this->animations.count(animation);
}