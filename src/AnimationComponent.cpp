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


AnimationComponent::AnimationComponent(const std::string& animationsFile)  :
	Component("Animation"),
	animationsFile(animationsFile)
{
	setAnimations(animationsFile);
}

std::ostream& operator<<(std::ostream& os, const AnimationComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.animationsFile;

	return os;
}

void AnimationComponent::setAnimations(const std::string& animationsFile)
{
	this->animations = Parsers::parseAnimations(animationsFile, this->animator);
	this->animationsFile = animationsFile;
}

void AnimationComponent::update(float deltaTime)
{
	this->animator.update(sf::seconds(deltaTime));
}

void AnimationComponent::animate(sf::Sprite& sprite)
{
	this->animator.animate(sprite);
}

void AnimationComponent::playAnimation(const Animation& animation, bool loop)
{
	if (this->hasAnimation(animation))
	{
		this->animator.playAnimation(animation, loop);
	}
}

void AnimationComponent::stopAnimation()
{
	this->animator.stopAnimation();
}

bool AnimationComponent::isPlayingAnimation() const
{
	return this->animator.isPlayingAnimation();
}

bool AnimationComponent::hasAnimation(const Animation& animation) const
{
	return this->animations.count(animation);
}