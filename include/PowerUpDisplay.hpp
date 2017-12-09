/*
Copyright (c) 2017 InversePalindrome
Nihil - PowerUpDisplay.hpp
InversePalindrome.com
*/


#pragma once

#include "Item.hpp"
#include "Animation.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <unordered_map>


struct PowerUpGraphics;
struct PowerUpGraphicsData;

using PowerUpData = std::unordered_map<Item, PowerUpGraphicsData>;

class PowerUpDisplay : public sf::Drawable, public sf::Transformable
{
public:
	PowerUpDisplay(ResourceManager& resourceManager);

	void update(float deltaTime);

	void addPowerUp(Item powerUp);
	void removePowerUp(Item powerUp);
	void clearPowerUps();

private:
	std::unordered_map<Item, PowerUpGraphics> powerUps;
	PowerUpData powerUpData;

	ResourceManager& resourceManager;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

struct PowerUpGraphics : public sf::Drawable, public sf::Transformable
{
	PowerUpGraphics(ResourceManager& resourceManager, Item item, const PowerUpData& data);

	sf::Sprite sprite;
	Animator animator;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

struct PowerUpGraphicsData
{
	PowerUpGraphicsData(const std::string& animationFile, const sf::IntRect& textureRect, float scale);

	std::string animationFile;
	sf::IntRect textureRect;
	float scale;
};