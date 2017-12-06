/*
Copyright (c) 2017 InversePalindrome
Nihil - SoundSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "SoundManager.hpp"


class SoundSystem : public System
{
public:
	SoundSystem(Entities& entities, Events& event, SoundManager& soundManager);

	virtual void update(float deltaTime) override;

private:
	SoundManager& soundManager;
	Entity listenerEntity;

	void playSound(SoundBuffersID soundBufferID, bool loop);
	void stopSound(SoundBuffersID soundID);

	void updateListenerPosition();
};