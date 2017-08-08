/*
Copyright (c) 2017 InversePalindrome
Nihil - SoundManager.cpp
InversePalindrome.com
*/


#include "SoundManager.hpp"

#include <SFML/Audio/Listener.hpp>

#include <iostream>


SoundManager::SoundManager(ResourceManager& resourceManager) :
	soundProperties(100.f, 1.f, 8.f, 200.f),
	musicProperties(100.f, 1.f, 8.f, 200.f),
	currentSoundID(0u),
	resourceManager(resourceManager)
{
}

SoundID SoundManager::getCurrentSoundID() const
{
	return this->currentSoundID;
}

const AudioProperties& SoundManager::getSoundProperties() const
{
	return this->soundProperties;
}

const AudioProperties& SoundManager::getMusicProperties() const
{
	return this->musicProperties;
}

void SoundManager::update()
{
	this->removeStoppedSounds<decltype(this->sounds)>(this->sounds);
	this->removeStoppedSounds<decltype(this->music)>(this->music);
}

void SoundManager::playSound(SoundBuffersID soundBuffersID, bool loop)
{
	auto sound = std::make_unique<sf::Sound>(this->resourceManager.getSound(soundBuffersID));

	this->applySoundProperties(sound, loop);

	this->sounds.emplace(this->currentSoundID, std::move(sound));

	this->sounds.at(this->currentSoundID)->play();

	++this->currentSoundID;
}

void SoundManager::stopSound(SoundID soundID)
{
	auto sound = this->sounds.find(soundID);

	if (sound != std::end(this->sounds))
	{
		sound->second->stop();
	}
}

void SoundManager::stopAllSounds()
{
	for (auto& sound : this->sounds)
	{
		sound.second->stop();
	}
}

void SoundManager::playMusic(const std::string& name, bool loop)
{
	auto music = std::make_unique<sf::Music>();

	if (!music->openFromFile("Resources/Music/" + name))
	{
		std::cerr << "Failed to open Music: " + name << std::endl;
	}

	music->setVolume(this->musicProperties.volume);
	music->setLoop(loop);

	this->music.emplace(name, std::move(music));
	this->music.at(name)->play();
}

void SoundManager::stopMusic(const std::string& name)
{
	auto music = this->music.find(name);

	if (music != std::end(this->music))
	{
		music->second->stop();
	}
}
void SoundManager::stopAllMusic()
{
	for (auto& music : this->music)
	{
		music.second->stop();
	}
}

void SoundManager::setSoundPosition(SoundID soundID, const sf::Vector3f& position)
{
	auto sound = this->sounds.find(soundID);

	if (sound != std::end(this->sounds))
	{
		sound->second->setPosition(position);
	}
}

void SoundManager::setListenerPosition(const sf::Vector3f& position)
{
	sf::Listener::setPosition(position);
}

void SoundManager::setListenerDirection(const sf::Vector3f& direction)
{
	sf::Listener::setDirection(direction);
}

void SoundManager::setSoundVolume(float volume)
{
	this->soundProperties.volume = volume;

	for (auto& sound : this->sounds)
	{
		sound.second->setVolume(volume);
	}
}

void SoundManager::setMusicVolume(float volume)
{
	this->musicProperties.volume = volume;

	for (auto& music : this->music)
	{
		music.second->setVolume(volume);
	}
}

void SoundManager::applySoundProperties(SoundPtr& sound, bool loop)
{
	sound->setVolume(soundProperties.volume);
	sound->setPitch(soundProperties.pitch);
	sound->setAttenuation(soundProperties.attenuation);
	sound->setMinDistance(soundProperties.minDistance);
	sound->setLoop(loop);
}