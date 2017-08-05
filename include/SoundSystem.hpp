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

	void changeSound(Entity entity, EntityState state);

	void emitSound(Entity entity, SoundBuffersID soundBufferID, bool loop);
	void stopSound(Entity entity);

	void updateListenerPosition(Entity entity);
};