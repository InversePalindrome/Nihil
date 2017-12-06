/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteComponent.cpp
InversePalindrome.com
*/


#include "SpriteComponent.hpp"
#include "SpriteParser.hpp"


SpriteComponent::SpriteComponent(ResourceManager& resourceManager, TexturesID textureID, const sf::Vector2f& scale) :
	Component("SpriteB"),
	textureID(textureID),
	sprite(resourceManager.getTexture(textureID)),
	resourceManager(&resourceManager)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
	sprite.setScale(scale);
}

SpriteComponent::SpriteComponent(ResourceManager& resourceManager, TexturesID textureID, const sf::IntRect& textureRect, const sf::Vector2f& scale) :
	Component("SpriteB"),
	textureID(textureID),
	sprite(resourceManager.getTexture(textureID), textureRect),
	resourceManager(&resourceManager)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
	sprite.setScale(scale);
}

SpriteComponent::SpriteComponent(ResourceManager& resourceManager, const std::string& fileName) :
	Component("SpriteC"),
	resourceManager(&resourceManager),
	fileName(fileName)
{
	Parsers::parseSprite(resourceManager, fileName, sprite);

	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
}

std::ostream& operator<<(std::ostream& os, const SpriteComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ';

	if (component.getName() == "SpriteB")
	{
		os << static_cast<std::size_t>(component.getTextureID()) << ' ' << component.sprite.getTextureRect().left << ' ' << component.sprite.getTextureRect().top
			<< ' ' << component.sprite.getTextureRect().width << ' ' << component.sprite.getTextureRect().height <<' ' << component.sprite.getScale().x << ' ' <<
			component.sprite.getScale().y;
	}
	else if (component.getName() == "SpriteC")
	{
		os  << component.fileName;
	}

	return os;
}

sf::FloatRect SpriteComponent::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

TexturesID SpriteComponent::getTextureID() const
{
	return this->textureID;
}

sf::Sprite& SpriteComponent::getSprite()
{
	return this->sprite;
}

void SpriteComponent::setSprite(const std::string& fileName)
{
	Parsers::parseSprite(*resourceManager, fileName, this->sprite);
}

void SpriteComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	states.transform *= this->getTransform();

	target.draw(this->sprite, states);
}