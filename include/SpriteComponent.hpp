/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class SpriteComponent : public Component, public sf::Drawable
{
	friend std::ostream& operator<<(std::ostream& os, const SpriteComponent& component);

public:
	SpriteComponent(ResourceManager& resourceManager, TexturesID textureID);
	SpriteComponent(ResourceManager& resourceManager, TexturesID textureID, const sf::IntRect& textureRect);
	SpriteComponent(ResourceManager& resourceManager, TexturesID textureID, const sf::IntRect& textureRect, const sf::Vector2f& scale);

	TexturesID getTextureID() const;
	sf::Sprite& getSprite();
	sf::Vector2f getPosition() const;
	const sf::Transform& getTransform() const;

	void setPosition(const sf::Vector2f& position);

private:
	TexturesID textureID;
	sf::Sprite sprite;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

std::ostream& operator<<(std::ostream& os, const SpriteComponent& component);
