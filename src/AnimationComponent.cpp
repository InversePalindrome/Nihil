/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationComponent.cpp
InversePalindrome.com
*/


#include "AnimationComponent.hpp"
#include "AnimationParser.hpp"
#include "FilePaths.hpp"

#include <fstream>
#include <sstream>


AnimationComponent::AnimationComponent(bool hasMultipleAnimations, const std::string& animationsFile)  :
	Component("Animation"),
	animationsFile(animationsFile),
	hasMultipleAnimations(hasMultipleAnimations)
{
	if (hasMultipleAnimations)
	{
		std::ifstream inFile(Path::animations / animationsFile);
		std::string line;

		while (std::getline(inFile, line))
		{
			std::istringstream iStream(line);

			std::string category;

			iStream >> category;

			if (category == "Animation")
			{
				std::size_t state = 0u, direction = 0u;
				float duration;

				iStream >> state >> direction >> duration;

				animations.emplace(std::make_pair(static_cast<EntityState>(state), static_cast<Direction>(direction)), std::make_pair(thor::FrameAnimation(), duration));
			}
			else if (category == "Frame")
			{
				float frameTime = 0.f;
				std::size_t left = 0u, top = 0u, width = 0u, length = 0u;

				iStream >> frameTime >> left >> top >> width >> length;

				animations.rbegin()->second.first.addFrame(frameTime, sf::IntRect(left, top, width, length));
			}
		}

		for (const auto& animation : animations)
		{
			stateAnimator.addAnimation(animation.first, animation.second.first, sf::seconds(animation.second.second));
		}

		if (!animations.empty())
		{
			stateAnimator.playAnimation(animations.begin()->first, true);
		}
	}
	else
	{
		thor::FrameAnimation animation;
		std::size_t animationID = 0u;
		float animationTime = 0.f;

		Path::parseFrameAnimations(animationsFile, animation, animationID, animationTime);

		singleAnimator.addAnimation(animationID, animation, sf::seconds(animationTime));
		singleAnimator.playAnimation(animationID, true);
	}
}

std::ostream& operator<<(std::ostream& os, const AnimationComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' <<  component.hasMultipleAnimations << ' ' << component.animationsFile;

	return os;
}

std::optional<std::pair<EntityState, Direction>> AnimationComponent::getCurrentAnimation() const
{
	if (this->stateAnimator.isPlayingAnimation() && this->hasMultipleAnimations)
	{
		return this->stateAnimator.getPlayingAnimation();
	}
	else
	{
		return {};
	}
}

void AnimationComponent::animate(sf::Sprite& sprite, float deltaTime)
{
	if (this->hasMultipleAnimations)
	{
		this->stateAnimator.animate(sprite);
		this->stateAnimator.update(sf::seconds(deltaTime));
	}
	else
	{
		this->singleAnimator.animate(sprite);
		this->singleAnimator.update(sf::seconds(deltaTime));
	}
}

void AnimationComponent::playAnimation(EntityState state, Direction direction, bool loop)
{
	this->stateAnimator.playAnimation(std::make_pair(state, direction), loop);
}

bool AnimationComponent::isPlayingAnimation() const
{
	return this->stateAnimator.isPlayingAnimation() || this->singleAnimator.isPlayingAnimation();
}

bool AnimationComponent::hasAnimation(const Animation& animation) const
{
	return this->animations.count(animation);
}