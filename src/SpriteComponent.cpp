/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteComponent.cpp
InversePalindrome.com
*/


#include "SpriteComponent.hpp"


SpriteComponent::SpriteComponent(sf::Texture& texture) :
	sprite(texture)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
}

SpriteComponent::SpriteComponent(sf::Texture& texture, const sf::IntRect& textureRect) :
	SpriteComponent(texture, textureRect, sf::Vector2f(1.f, 1.f))
{
}

SpriteComponent::SpriteComponent(sf::Texture& texture, const sf::IntRect& textureRect, const sf::Vector2f& scale) :
	sprite(texture, textureRect)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
	sprite.setScale(scale);
}

sf::Sprite& SpriteComponent::getSprite()
{
	return this->sprite;
}

void SpriteComponent::setPosition(const sf::Vector2f& position)
{
	this->sprite.setPosition(position);
}

void SpriteComponent::draw(sf::RenderTarget& target) 
{
	target.draw(this->sprite);
}