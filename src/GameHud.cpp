/*
Copyright (c) 2017 InversePalindrome
Nihil - GameHud.cpp
InversePalindrome.com
*/


#include "GameHud.hpp"


GameHud::GameHud(ResourceManager& resourceManager) :
	numberOfHitpoints(5u),
	coinDisplayNumber(0u),
	heartSprite(),
	coinSprite(resourceManager.getTexture(TexturesID::Coin)),
	coinText(std::to_string(coinDisplayNumber), resourceManager.getFont(FontsID::BITWONDER), 75u)
{
	auto& heartTexture = resourceManager.getTexture(TexturesID::Heart);
	heartTexture.setRepeated(true);

	heartSprite.setTexture(heartTexture);
	heartSprite.setScale(0.65f, 0.65f);
	heartSprite.setPosition(150.f, 120.f);

	coinSprite.setScale(1.5f, 1.5f);
	coinSprite.setPosition(1700.f, 120.f);

	coinText.setOutlineThickness(5.f);
	coinText.setFillColor(sf::Color(156u, 20u, 107u));
	coinText.setOutlineColor(sf::Color(75u, 0u, 130u));
	coinText.setPosition(1800.f, 120.f);
	
	thor::FrameAnimation frameAnimation;

	frameAnimation.addFrame(1.f, sf::IntRect(0u, 0u, 60u, 60u));
	frameAnimation.addFrame(1.f, sf::IntRect(60u, 0u, 60u, 60u));
	frameAnimation.addFrame(1.f, sf::IntRect(0u, 60u, 60u, 60u));
	frameAnimation.addFrame(1.f, sf::IntRect(60u, 60u, 60u, 60u));

	animator.addAnimation("Animation", frameAnimation, sf::seconds(0.5f));

	animator.playAnimation("Animation", true);
}

void GameHud::update(float deltaTime)
{
	this->animator.animate(this->coinSprite);
	this->animator.update(sf::seconds(deltaTime));
}

std::size_t GameHud::getNumberOfHitpoints() const
{
	return this->numberOfHitpoints;
}

std::size_t GameHud::getCoinDisplayNumber() const
{
	return this->coinDisplayNumber;
}

void GameHud::setNumberOfHitpoints(std::size_t numberOfHitpoints)
{
	this->numberOfHitpoints = numberOfHitpoints;

	this->heartSprite.setTextureRect(sf::IntRect(0.f, 0.f, this->heartSprite.getTexture()->getSize().x * numberOfHitpoints, this->heartSprite.getTexture()->getSize().y));
}

void GameHud::setCoinDisplayNumber(std::size_t displayNumber)
{
	this->coinDisplayNumber = displayNumber;

	this->coinText.setString(std::to_string(this->coinDisplayNumber));
}

void GameHud::addToCoinDisplayNumber()
{
	++this->coinDisplayNumber;

	this->coinText.setString(std::to_string(this->coinDisplayNumber));
}

void GameHud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = this->getTransform();

	target.draw(this->heartSprite, states);
	target.draw(this->coinSprite, states);

	target.draw(this->coinText, states);
}