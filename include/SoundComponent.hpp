/*
Copyright (c) 2017 InversePalindrome
Nihil - SoundComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "SoundManager.hpp"


class SoundComponent
{
public:
	SoundComponent();

	SoundID getSoundID() const;

	void setSoundID(SoundID soundID);

private:
	SoundID soundID;
};