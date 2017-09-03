/*
Copyright (c) 2017 InversePalindrome
Nihil - SoundComponent.cpp
InversePalindrome.com
*/


#include "SoundComponent.hpp"


SoundComponent::SoundComponent() :
	Component(ComponentID::Sound),
	soundID(-1)
{
}

std::ostream& SoundComponent::operator<<(std::ostream& os)
{
	os << this->soundID;

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