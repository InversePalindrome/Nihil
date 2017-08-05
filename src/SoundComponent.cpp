/*
Copyright (c) 2017 InversePalindrome
Nihil - SoundComponent.cpp
InversePalindrome.com
*/


#include "SoundComponent.hpp"


SoundComponent::SoundComponent() :
	soundID(-1u)
{
}

SoundID SoundComponent::getSoundID() const
{
	return this->soundID;
}

void SoundComponent::setSoundID(SoundID soundID)
{
	this->soundID = soundID;
}