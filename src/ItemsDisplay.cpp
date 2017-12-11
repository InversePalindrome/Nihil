/*
Copyright (c) 2017 InversePalindrome
Nihil - ItemsDisplay.cpp
InversePalindrome.com
*/


#include "ItemsDisplay.hpp"
#include "FilePaths.hpp"
#include "AnimationParser.hpp"
#include "TextStyleParser.hpp"

#include <fstream>
#include <sstream>


ItemsDisplay::ItemsDisplay(ResourceManager& resourceManager) :
	isVisible(false)
{
	loadItems(resourceManager, Path::miscellaneous / "ItemsDisplay.txt");

	for (auto& item : itemsData)
	{
		const auto& animations = Parsers::parseAnimations(item.second.animationFile, item.second.animator);

		if (!animations.empty())
		{
			item.second.animator.playAnimation(animations.cbegin()->first, true);
		}
	}
}

ItemData& ItemsDisplay::operator[](Item item)
{
	return this->itemsData[item];
}

void ItemsDisplay::update(float deltaTime)
{
	if (this->isVisible)
	{
		for (auto& itemData : this->itemsData)
		{
			if (itemData.second.animator.isPlayingAnimation())
			{
				itemData.second.animator.update(sf::seconds(deltaTime));
				itemData.second.animator.animate(itemData.second.sprite);
			}
		}
	}
}

bool ItemsDisplay::getVisibility() const
{
	return this->isVisible;
}

void ItemsDisplay::setVisibility(bool isVisible)
{
	this->isVisible = isVisible;
}

bool ItemsDisplay::hasItem(Item item) const
{
	return this->itemsData.count(item);
}

void ItemsDisplay::loadItems(ResourceManager& resourceManager, const std::string& fileName)
{
	std::ifstream inFile(fileName);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::size_t item = 0u;

		iStream >> item;

		this->itemsData.emplace(Item{ item }, ItemData(resourceManager, line));
	}
}

void ItemsDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (this->isVisible)
	{
		states.transform *= this->getTransform();

		for (const auto& item : this->itemsData)
		{
			target.draw(item.second, states);
		}
	}
}

ItemData::ItemData(ResourceManager& resourceManager, const std::string& itemData) :
	quantity(0u)
{
	std::istringstream iStream(itemData);

	std::size_t item = 0u, textureID = 0u;
	float scale = 0.f, xPosition = 0.f, yPosition = 0.f, textOffset = 0.f;

	iStream >> item >> textureID >> scale >> xPosition >> yPosition >> textOffset >> maxQuantity >> animationFile;

	sprite.setTexture(resourceManager.getTexture(TexturesID{ textureID }));
	sprite.setScale(scale, scale);
	sprite.setPosition(xPosition, yPosition);

	Parsers::parseStyle(resourceManager, "ItemStyle.txt", info);

	info.setPosition( xPosition, yPosition + textOffset );
	info.setString(std::to_string(quantity) + " / " + std::to_string(maxQuantity));
}

void ItemData::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = this->getTransform();

	target.draw(this->sprite, states);
	target.draw(this->info, states);
}
