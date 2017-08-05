/*
Copyright (c) 2017 InversePalindrome
Nihil - ResourceManager.cpp
InversePalindrome.com
*/


#include "ResourceManager.hpp"

#include <Thor/Resources/SfmlLoaders.hpp>

#include <fstream>


ResourceManager::ResourceManager() :
	ResourceManager("")
{	
}

ResourceManager::ResourceManager(const std::string& resourcesFilePath) 
{
	resourceFactory.emplace("Texture", [this]
	(std::size_t ID, const std::string& resourceFilePath)
	{ textures.acquire(static_cast<TexturesID>(ID), thor::Resources::fromFile<sf::Texture>(resourceFilePath)); });

	resourceFactory.emplace("Image", [this]
	(std::size_t ID, const std::string& resourceFilePath)
	{ images.acquire(static_cast<ImagesID>(ID), thor::Resources::fromFile<sf::Image>(resourceFilePath)); });

	resourceFactory.emplace("Font", [this]
	(std::size_t ID, const std::string& resourceFilePath)
	{ fonts.acquire(static_cast<FontsID>(ID), thor::Resources::fromFile<sf::Font>(resourceFilePath)); });

	resourceFactory.emplace("Sound", [this]
	(std::size_t ID, const std::string& resourceFilePath)
	{ sounds.acquire(static_cast<SoundBuffersID>(ID), thor::Resources::fromFile<sf::SoundBuffer>(resourceFilePath)); });

	loadResources(resourcesFilePath);
}

void ResourceManager::loadResources(const std::string& resourcesFilePath)
{
	std::ifstream inFile(resourcesFilePath);

	std::string resourceType;
	std::size_t resourceID;
	std::string filePath;

	while (inFile >> resourceType >> resourceID >> filePath)
	{
		this->resourceFactory.at(resourceType)(resourceID, filePath);
	}
}

sf::Texture& ResourceManager::getTexture(TexturesID textureID)
{
	return this->textures[textureID];
}

sf::Image& ResourceManager::getImage(ImagesID imageID)
{
	return this->images[imageID];
}

sf::Font& ResourceManager::getFont(FontsID fontID)
{
	return this->fonts[fontID];
}

sf::SoundBuffer& ResourceManager::getSound(SoundBuffersID soundBuffersID)
{
	return this->sounds[soundBuffersID];
}