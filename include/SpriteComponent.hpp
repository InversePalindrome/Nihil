/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class SpriteComponent : public Component, public sf::Drawable
{
public:
	SpriteComponent(sf::Texture& texture);
	SpriteComponent(sf::Texture& texuture, const sf::IntRect& textureRect);
	SpriteComponent(sf::Texture& texture, const sf::IntRect& textureRect, const sf::Vector2f& scale);

	virtual std::ostream& operator<<(std::ostream& os) override;

	sf::Sprite& getSprite();
	sf::Vector2f getPosition() const;
	const sf::Transform& getTransform() const;

	void setPosition(const sf::Vector2f& position);

private:
	sf::Sprite sprite;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
