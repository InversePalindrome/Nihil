/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteComponent.cpp
InversePalindrome.com
*/


#include "SpriteComponent.hpp"


SpriteComponent::SpriteComponent(sf::Texture& texture) :
	sprite(texture)
{
}

void SpriteComponent::setPosition(const sf::Vector2f& position)
{
	this->sprite.setPosition(position);
}

void SpriteComponent::draw(sf::RenderTarget& target) 
{
	target.draw(this->sprite);
}