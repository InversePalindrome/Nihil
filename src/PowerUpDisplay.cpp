/*
Copyright (c) 2017 InversePalindrome
Nihil - PowerUpDisplay.cpp
InversePalindrome.com
*/


#include "PowerUpDisplay.hpp"
#include "FilePaths.hpp"
#include "AnimationParser.hpp"

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

		std::size_t itemID = 0u;
		int left = 0, top = 0, width = 0, height = 0;
		std::string animationFile;
		float scale = 0.f;

		iStream >> itemID >> animationFile >> left >> top >> width >> height >> scale;

		powerUpData.emplace(Item{ itemID }, PowerUpGraphicsData(animationFile, { left, top, width, height }, scale));
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

void PowerUpDisplay::clearPowerUps()
{
	this->powerUps.clear();
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
	
	const auto& animations = Parsers::parseAnimations("CoinAnimations.txt", animator);

	if (!animations.empty())
	{
		animator.playAnimation(animations.cbegin()->first, true);
	}
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