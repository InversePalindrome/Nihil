/*
Copyright (c) 2017 InversePalindrome
Nihil - SoundManager.hpp
InversePalindrome.com
*/


#pragma once

#include "AudioProperties.hpp"
#include "ResourceManager.hpp"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include <string>
#include <memory>
#include <unordered_map>


class SoundManager
{
	using SoundPtr = std::unique_ptr<sf::Sound>;
	using MusicPtr = std::unique_ptr<sf::Music>;

public:
	SoundManager(ResourceManager& resourceManager);
	SoundManager(const SoundManager& soundManager) = delete;
	SoundManager& operator=(const SoundManager& soundManager) = delete;

	const AudioProperties& getSoundProperties() const;
	const AudioProperties& getMusicProperties() const;

	void update();

	void playSound(SoundBuffersID soundBufferID, bool loop);
	void stopSound(SoundBuffersID soundID);
	void stopAllSounds();

	void playMusic(const std::string& name, bool loop);
	void stopMusic(const std::string& name);
	void stopAllMusic();

	void setSoundPosition(SoundBuffersID soundID, const sf::Vector3f& position);

	void setListenerPosition(const sf::Vector3f& position);
	void setListenerDirection(const sf::Vector3f& direction);

	void setSoundVolume(float volume);
	void setMusicVolume(float volume);

private:
	AudioProperties soundProperties;
	AudioProperties musicProperties;

	ResourceManager& resourceManager;

	std::unordered_multimap<SoundBuffersID, SoundPtr> sounds;
	std::unordered_map<std::string, MusicPtr> music;

	void applySoundProperties(SoundPtr& sound, bool loop);

	template<typename T>
	void removeStoppedSounds(T& map);
};

template<typename T>
void SoundManager::removeStoppedSounds(T& map)
{
	for (auto itr = std::begin(map); itr != std::end(map);)
	{
		if (!itr->second->getStatus())
		{
			itr = map.erase(itr);
			continue;
		}

		++itr;
	}
}