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
		thor::FrameAnimation animation;
		std::size_t animationID = 0u;
		float animationTime = 0.f;

		Parsers::parseFrameAnimations(item.second.animationFile, animation, animationID, animationTime);
		item.second.animator.addAnimation(animationID, animation, sf::seconds(animationTime));
		item.second.animator.playAnimation(animationID, true);
	}
}

ItemsGraphics& ItemsDisplay::operator[](Item item)
{
	return this->itemsData[item];
}

void ItemsDisplay::update(float deltaTime)
{
	if (this->isVisible)
	{
		for (auto& item : this->itemsData)
		{
			if (item.second.animator.isPlayingAnimation())
			{
				item.second.animator.update(sf::seconds(deltaTime));
				item.second.animator.animate(item.second.sprite);
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

		this->itemsData.emplace(static_cast<Item>(item), ItemsGraphics(resourceManager, line));
	}
}

void ItemsDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (this->isVisible)
	{
		states.transform = this->getTransform();

		for (const auto& item : this->itemsData)
		{
			target.draw(item.second, states);
		}
	}
}

ItemsGraphics::ItemsGraphics(ResourceManager& resourceManager, const std::string& itemData) :
	quantity(0u)
{
	std::istringstream iStream(itemData);

	std::size_t item = 0u, textureID = 0u;
	float scale = 0.f, xPosition = 0.f, yPosition = 0.f, textOffset = 0.f;
	std::string iAnimationFile;

	iStream >> item >> textureID >> scale >> xPosition >> yPosition >> textOffset >> maxQuantity >> iAnimationFile;

	animationFile = iAnimationFile;

	sprite.setTexture(resourceManager.getTexture(static_cast<TexturesID>(textureID)));
	sprite.setScale(sf::Vector2f(scale, scale));
	sprite.setPosition(xPosition, yPosition);

	Parsers::parseStyle(resourceManager, "ItemStyle.txt", info);

	info.setPosition(sf::Vector2f(xPosition, yPosition + textOffset));
	info.setString(std::to_string(quantity) + " / " + std::to_string(maxQuantity));
}

void ItemsGraphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = this->getTransform();

	target.draw(this->sprite, states);
	target.draw(this->info, states);
}
