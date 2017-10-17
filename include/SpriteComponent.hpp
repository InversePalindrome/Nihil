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
#include <SFML/Graphics/Transformable.hpp>

#include <string>


class SpriteComponent : public Component, public sf::Transformable, public sf::Drawable
{
	friend std::ostream& operator<<(std::ostream& os, const SpriteComponent& component);

public:
	SpriteComponent(ResourceManager& resourceManager, TexturesID textureID, const sf::Vector2f& scale);
	SpriteComponent(ResourceManager& resourceManager, TexturesID textureID, const sf::IntRect& textureRect, const sf::Vector2f& scale);
	SpriteComponent(ResourceManager& resourceManager, const std::string& fileName);

	sf::FloatRect getGlobalBounds() const;

	TexturesID getTextureID() const;
	sf::Sprite& getSprite();

	void parseSprite(const std::string& fileName);

private:
	TexturesID textureID;
	sf::Sprite sprite;

	ResourceManager* resourceManager;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

std::ostream& operator<<(std::ostream& os, const SpriteComponent& component);
