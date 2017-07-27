/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteComponent.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class SpriteComponent 
{
public:
	SpriteComponent(sf::Texture& texture);

	void draw(sf::RenderTarget& target);

	void setPosition(const sf::Vector2f& position);

private:
	sf::Sprite sprite;
};
