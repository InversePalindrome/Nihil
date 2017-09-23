/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteComponent.cpp
InversePalindrome.com
*/


#include "SpriteComponent.hpp"


SpriteComponent::SpriteComponent(ResourceManager& resourceManager, TexturesID textureID) :
	Component("SpriteA"),
	textureID(textureID),
	sprite(resourceManager.getTexture(textureID))
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
}

SpriteComponent::SpriteComponent(ResourceManager& resourceManager, TexturesID textureID, const sf::IntRect& textureRect, const sf::Vector2f& scale) :
	Component("SpriteB"),
	textureID(textureID),
	sprite(resourceManager.getTexture(textureID), textureRect)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
	sprite.setScale(scale);
}

std::ostream& operator<<(std::ostream& os, const SpriteComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << static_cast<std::size_t>(component.getTextureID()) << ' ' << component.sprite.getTextureRect().left << 
		' ' << component.sprite.getTextureRect().top <<  ' ' << component.sprite.getTextureRect().width <<  ' ' << component.sprite.getTextureRect().height <<
		' ' << component.sprite.getScale().x << ' ' << component.sprite.getScale().y;

	return os;
}

TexturesID SpriteComponent::getTextureID() const
{
	return this->textureID;
}

sf::Sprite& SpriteComponent::getSprite()
{
	return this->sprite;
}

sf::Vector2f SpriteComponent::getPosition() const
{
	return this->sprite.getPosition();
}

const sf::Transform& SpriteComponent::getTransform() const
{
	return this->sprite.getTransform();
}

void SpriteComponent::setPosition(const sf::Vector2f& position)
{
	this->sprite.setPosition(position);
}

void SpriteComponent::setRotation(float rotation)
{
	this->sprite.setRotation(rotation);
}

void SpriteComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(this->sprite, states);
}