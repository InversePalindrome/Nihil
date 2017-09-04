/*
Copyright (c) 2017 InversePalindrome
Nihil - SoundComponent.cpp
InversePalindrome.com
*/


#include "SoundComponent.hpp"


SoundComponent::SoundComponent() :
	Component("Sound"),
	soundID(-1)
{
}

std::ostream& operator<<(std::ostream& os, const SoundComponent& component)
{
	os << component.getEntity() << ' ' << component.getName() << ' ' << component.soundID;

	return os;
}

SoundID SoundComponent::getSoundID() const
{
	return this->soundID;
}

void SoundComponent::setSoundID(SoundID soundID)
{
	this->soundID = soundID;
}