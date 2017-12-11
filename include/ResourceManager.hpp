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

 
enum class TexturesID : std::size_t
{
	SplashScreen, StartBackground, Particles, Character, Mushroom, RedMonster, MenuBackground,                 
    City, Desert, MushroomLand, PinkTown, FlowerLand, DarkTown, Coin, Heart, Laser, Gems, MistZone,
	Artillery, PowerUps, Keys, DialogBox, Panels, RectangularBoxes, Jungle, Moon, PinkRocks, Plain, SkyZone,
	IceZone, Tileset, Bubble, Platforms, Skeleton, Goblin, Wizard, GreenMonster, PurpleMonster, Cannons
};
enum class ImagesID : std::size_t
{ 
	RedMonster
};
enum class FontsID : std::size_t
{ 
	BITWONDER, CODE, Roboto, Changa
};
enum class SoundBuffersID : std::size_t
{ 
	Jump, Footsteps, Coin, Laser, SmallExplosion, Gem, Achievement, Swimming, Trampoline, CannonBall
};

class ResourceManager
{
public:
	ResourceManager() = default;
	ResourceManager(const std::string& resourcesFilePath);
	ResourceManager(const ResourceManager& resourceManager) = delete;
	ResourceManager& operator=(const ResourceManager& resourceManager) = delete;

	void loadResources(const std::string& resourcesFilePath);

	sf::Texture& getTexture(TexturesID textureID);
	sf::Image& getImage(ImagesID imageID);
	sf::Font& getFont(FontsID fontID);
	sf::SoundBuffer& getSound(SoundBuffersID soundBuffersID);

private:
	thor::ResourceHolder<sf::Texture, TexturesID> textures;
	thor::ResourceHolder<sf::Image, ImagesID> images;
	thor::ResourceHolder<sf::Font, FontsID> fonts;
	thor::ResourceHolder<sf::SoundBuffer, SoundBuffersID> sounds;

	std::unordered_map<std::string, std::function<void(std::size_t, const std::string&)>> resourceFactory;
};