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
public:
	SoundComponent();

	virtual std::ostream& operator<<(std::ostream& os) override;

	SoundID getSoundID() const;

	void setSoundID(SoundID soundID);

private:
	SoundID soundID;
};