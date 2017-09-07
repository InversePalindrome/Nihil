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
	events.subscribe<StateChanged>([this](const auto& event) { changeSound(event.entity, event.state); });
	events.subscribe<EmitSound>([&soundManager](const auto& event) { soundManager.playSound(event.soundBuffer, event.loop); });
}

void SoundSystem::update(float deltaTime)
{
	this->entities.for_each<SoundComponent, PositionComponent>(
		[this](auto entity, auto& sound, const auto& position)
	{
	    this->soundManager.setSoundPosition(sound.getSoundID(), sf::Vector3f(position.getPosition().x, position.getPosition().y, 0.f));

		this->updateListenerPosition(entity);
	});
}

void SoundSystem::changeSound(Entity entity, EntityState state)
{
	if (entity.has_component<SoundComponent>())
	{
		switch (state)
		{
		case EntityState::Idle:
			this->stopSound(entity);
			break;
		case EntityState::Walking:
			this->emitSound(entity, SoundBuffersID::Footsteps, true);
			break;
		case EntityState::Jumping:
			this->stopSound(entity);
			this->emitSound(entity, SoundBuffersID::Jump, false);
			break;
		case EntityState::Attacking:
			break;
		}
	}
}

void SoundSystem::emitSound(Entity entity, SoundBuffersID soundBufferID, bool loop)
{
	auto& sound = entity.get_component<SoundComponent>();

	sound.setSoundID(this->soundManager.getCurrentSoundID());

	this->soundManager.playSound(soundBufferID, loop);
}

void SoundSystem::stopSound(Entity entity)
{
	this->soundManager.stopSound(entity.get_component<SoundComponent>().getSoundID());
}

void SoundSystem::updateListenerPosition(Entity entity)
{
	if (entity.has_component<ControllableComponent>())
	{
		const auto& position = entity.get_component<PositionComponent>().getPosition();

		this->soundManager.setListenerPosition(sf::Vector3f(position.x, 0.f, position.y));
    }
}