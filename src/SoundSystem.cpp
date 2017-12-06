/*
Copyright (c) 2017 InversePalindrome
Nihil - SoundSystem.cpp
InversePalindrome.com
*/


#include "SoundSystem.hpp"


SoundSystem::SoundSystem(Entities& entities, Events& events, SoundManager& soundManager) :
	System(entities, events),
	soundManager(soundManager)
{
	events.subscribe<PlaySound>([&soundManager](const auto& event) { soundManager.playSound(event.soundBuffer, event.loop); });
	events.subscribe<entityplus::component_added<Entity, ControllableComponent>>([this](const auto& event)
	{
		listenerEntity = event.entity;
	});
}

void SoundSystem::update(float deltaTime)
{
}

void SoundSystem::playSound(SoundBuffersID soundID, bool loop)
{
	this->soundManager.playSound(soundID, loop);
}

void SoundSystem::stopSound(SoundBuffersID soundID)
{
	this->soundManager.stopSound(soundID);
}

void SoundSystem::updateListenerPosition()
{
	if (this->listenerEntity.sync() && this->listenerEntity.has_component<PositionComponent>())
	{
		const auto& position = this->listenerEntity.get_component<PositionComponent>().getPosition();

		this->soundManager.setListenerPosition({ position.x, 0.f, position.y });
    }
}