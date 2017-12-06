/*
Copyright (c) 2017 InversePalindrome
Nihil - ResourceManager.cpp
InversePalindrome.com
*/


#include "ResourceManager.hpp"
#include "FilePaths.hpp"

#include <Thor/Resources/SfmlLoaders.hpp>

#include <fstream>


ResourceManager::ResourceManager(const std::string& resourcesFilePath) 
{
	resourceFactory.emplace("Textures", [this]
	(std::size_t ID, const std::string& resourceFilePath)
	{ textures.acquire(TexturesID{ID}, thor::Resources::fromFile<sf::Texture>(resourceFilePath)); });

	resourceFactory.emplace("Images", [this]
	(std::size_t ID, const std::string& resourceFilePath)
	{ images.acquire(ImagesID{ ID }, thor::Resources::fromFile<sf::Image>(resourceFilePath)); });

	resourceFactory.emplace("Fonts", [this]
	(std::size_t ID, const std::string& resourceFilePath)
	{ fonts.acquire(FontsID{ ID }, thor::Resources::fromFile<sf::Font>(resourceFilePath)); });

	resourceFactory.emplace("Sounds", [this]
	(std::size_t ID, const std::string& resourceFilePath)
	{ sounds.acquire(SoundBuffersID{ ID }, thor::Resources::fromFile<sf::SoundBuffer>(resourceFilePath)); });

	loadResources(resourcesFilePath);
}

void ResourceManager::loadResources(const std::string& resourcesFilePath)
{
	std::ifstream inFile(Path::miscellaneous / resourcesFilePath);

	std::string resourceType;
	std::size_t resourceID;
	std::string fileName;

	while (inFile >> resourceType >> resourceID >> fileName)
	{
		this->resourceFactory[resourceType](resourceID, Path::resources / resourceType + '/' + fileName);
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