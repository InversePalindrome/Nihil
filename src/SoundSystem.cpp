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
	events.subscribe<DestroyEntity>([this](const auto& event) { stopSound(event.entity); });
}

void SoundSystem::update(float deltaTime)
{
	this->entities.for_each<SoundComponent, PositionComponent>(
		[this](auto entity, const auto& sound, const auto& position)
	{
		this->soundManager.setSoundPosition(sound.getSoundID(), { position.getPosition().x, position.getPosition().y, 0.f });

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
			this->stopSound(entity);
			this->emitSound(entity, SoundBuffersID::Footsteps, true);
			break;
		case EntityState::Swimming:
			this->stopSound(entity);
			this->emitSound(entity, SoundBuffersID::Swimming, true);
			break;
		}
	}
}

void SoundSystem::emitSound(Entity entity, SoundBuffersID soundBufferID, bool loop)
{
	if (entity.has_component<SoundComponent>())
	{
		auto& sound = entity.get_component<SoundComponent>();

		sound.setSoundID(this->soundManager.getCurrentSoundID());

		this->soundManager.playSound(soundBufferID, loop);
	}
}

void SoundSystem::stopSound(Entity entity)
{
	if (entity.has_component<SoundComponent>())
	{
		this->soundManager.stopSound(entity.get_component<SoundComponent>().getSoundID());
	}
}

void SoundSystem::updateListenerPosition(Entity entity)
{
	if (entity.has_component<ControllableComponent>())
	{
		const auto& position = entity.get_component<PositionComponent>().getPosition();

		this->soundManager.setListenerPosition({ position.x, 0.f, position.y });
    }
}