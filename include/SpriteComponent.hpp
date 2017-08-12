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


class SpriteComponent : public sf::Drawable
{
public:
	SpriteComponent(sf::Texture& texture);
	SpriteComponent(sf::Texture& texuture, const sf::IntRect& textureRect);
	SpriteComponent(sf::Texture& texture, const sf::IntRect& textureRect, const sf::Vector2f& scale);

	sf::Sprite& getSprite();
	const sf::Transform& getTransform() const;

	void setPosition(const sf::Vector2f& position);

private:
	sf::Sprite sprite;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
