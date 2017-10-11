/*
Copyright (c) 2017 InversePalindrome
Nihil - PowerUpDisplay.cpp
InversePalindrome.com
*/


#include "PowerUpDisplay.hpp"
#include "FilePaths.hpp"
#include "AnimationParser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


PowerUpDisplay::PowerUpDisplay(ResourceManager& resourceManager) :
	resourceManager(resourceManager)
{
	std::ifstream inFile(Path::miscellaneous / "PowerUpsDisplay.txt");
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::size_t itemID = 0u, left = 0u, top = 0u, width = 0u, height = 0u;
		std::string animationFile;
		float scale = 0.f;

		iStream >> itemID >> animationFile >> left >> top >> width >> height >> scale;

		powerUpData.emplace(static_cast<Item>(itemID),
			PowerUpGraphicsData(animationFile, sf::IntRect(left, top, width, height), scale));
	}
}

void PowerUpDisplay::update(float deltaTime)
{
	for (auto& powerUpGraphic : this->powerUps)
	{
		powerUpGraphic.second.animator.update(sf::seconds(deltaTime));
		powerUpGraphic.second.animator.animate(powerUpGraphic.second.sprite);
	}
}

void PowerUpDisplay::addPowerUp(Item powerUp)
{
	this->powerUps.emplace(powerUp, PowerUpGraphics(this->resourceManager, powerUp, this->powerUpData));
	this->powerUps.at(powerUp).setPosition(
	{ this->powerUps.size() * this->powerUps.at(powerUp).sprite.getGlobalBounds().width, 0.f });
}

void PowerUpDisplay::removePowerUp(Item powerUp)
{
	this->powerUps.erase(powerUp);
}

void PowerUpDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = this->getTransform();

	for (const auto& powerUpGraphic : this->powerUps)
	{
		target.draw(powerUpGraphic.second, states);
	}
}

PowerUpGraphics::PowerUpGraphics(ResourceManager& resourceManager, Item item, const PowerUpData& powerUpData) 
{
	const auto& graphicData = powerUpData.at(item);

	sprite.setTexture(resourceManager.getTexture(TexturesID::PowerUps));
	sprite.setTextureRect(graphicData.textureRect);
	sprite.setScale({ graphicData.scale, graphicData.scale });
	
	thor::FrameAnimation animation;
	std::size_t animationID = 0u;
	float animationTime = 0.f;

	Parsers::parseFrameAnimations(graphicData.animationFile, animation, animationID, animationTime);
	animator.addAnimation(animationID, animation, sf::seconds(animationTime));
	animator.playAnimation(animationID, true);
}

void PowerUpGraphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	
	target.draw(this->sprite, states);
}

PowerUpGraphicsData::PowerUpGraphicsData(const std::string& animationFile, 
	const sf::IntRect& textureRect, float scale) :
	animationFile(animationFile),
	textureRect(textureRect),
	scale(scale)
{
}