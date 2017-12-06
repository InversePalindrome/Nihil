/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Renderable.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <string>


class SpriteComponent : public Component, public Renderable
{
	friend std::ostream& operator<<(std::ostream& os, const SpriteComponent& component);

public:
	SpriteComponent(ResourceManager& resourceManager, TexturesID textureID, const sf::Vector2f& scale);
	SpriteComponent(ResourceManager& resourceManager, TexturesID textureID, const sf::IntRect& textureRect, const sf::Vector2f& scale);
	SpriteComponent(ResourceManager& resourceManager, const std::string& fileName);

	sf::FloatRect getGlobalBounds() const;

	TexturesID getTextureID() const;
	sf::Sprite& getSprite();

	void setSprite(const std::string& fileName);

private:
	TexturesID textureID;
	sf::Sprite sprite;
	std::string fileName;

	ResourceManager* resourceManager;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

std::ostream& operator<<(std::ostream& os, const SpriteComponent& component);
