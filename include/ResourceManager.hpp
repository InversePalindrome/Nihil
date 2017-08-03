/*
Copyright (c) 2017 InversePalindrome
Nihil - ResourceManager.hpp
InversePalindrome.com
*/


#pragma once

#include <Thor/Resources/ResourceHolder.hpp>
#include <Thor/Resources/ResourceLoader.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <functional>
#include <unordered_map>

 
enum class TexturesID { SplashScreen, StartBackground, Particles, Character, Mushroom };
enum class ImagesID { PlayButton, SettingsButton, CharactersButton, BackButton };
enum class FontsID { BITWONDER };
enum class SoundsID {};

class ResourceManager
{
public:
	ResourceManager();
	ResourceManager(const std::string& resourcesFilePath);

	void loadResources(const std::string& resourcesFilePath);

	sf::Texture& getTexture(TexturesID textureID);
	sf::Image& getImage(ImagesID imageID);
	sf::Font& getFont(FontsID fontID);
	sf::SoundBuffer& getSound(SoundsID soundID);

private:
	thor::ResourceHolder<sf::Texture, TexturesID> textures;
	thor::ResourceHolder<sf::Image, ImagesID> images;
	thor::ResourceHolder<sf::Font, FontsID> fonts;
	thor::ResourceHolder<sf::SoundBuffer, SoundsID> sounds;

	std::unordered_map<std::string, std::function<void(std::size_t, const std::string&)>> resourceFactory;
};