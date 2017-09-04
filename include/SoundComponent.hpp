/*
Copyright (c) 2017 InversePalindrome
Nihil - SoundComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "SoundManager.hpp"
#include "Component.hpp"


class SoundComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const SoundComponent& component);

public:
	SoundComponent();

	SoundID getSoundID() const;

	void setSoundID(SoundID soundID);

private:
	SoundID soundID;
};

std::ostream& operator<<(std::ostream& os, const SoundComponent& component);