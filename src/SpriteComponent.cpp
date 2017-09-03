/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteComponent.cpp
InversePalindrome.com
*/


#include "SpriteComponent.hpp"


SpriteComponent::SpriteComponent(sf::Texture& texture) :
	Component(ComponentID::Sprite),
	sprite(texture)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
}

SpriteComponent::SpriteComponent(sf::Texture& texture, const sf::IntRect& textureRect) :
	SpriteComponent(texture, textureRect, sf::Vector2f(1.f, 1.f))
{
}

SpriteComponent::SpriteComponent(sf::Texture& texture, const sf::IntRect& textureRect, const sf::Vector2f& scale) :
	Component(ComponentID::Sprite),
	sprite(texture, textureRect)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
	sprite.setScale(scale);
}

std::ostream& SpriteComponent::operator<<(std::ostream& os)
{
	os << this->sprite.getTextureRect().left << ' ' << this->sprite.getTextureRect().top <<  ' ' << this->sprite.getTextureRect().width <<  ' ' <<
		this->sprite.getTextureRect().height << ' ' << this->sprite.getScale().x << ' ' << this->sprite.getScale().y;

	return os;
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

void SpriteComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(this->sprite, states);
}