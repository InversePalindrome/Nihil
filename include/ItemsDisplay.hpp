/*
Copyright (c) 2017 InversePalindrome
Nihil - ItemsDisplay.hpp
InversePalindrome.com
*/


#pragma once

#include "Item.hpp"
#include "Renderable.hpp"
#include "ResourceManager.hpp"

#include <Thor/Animations/Animator.hpp>
#include <Thor/Animations/FrameAnimation.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <string>
#include <unordered_map>


struct ItemsGraphics;

class ItemsDisplay : public Renderable
{
public:
	ItemsDisplay(ResourceManager& resourceManager);

	ItemsGraphics& operator[](Item item);

	void update(float deltaTime);

	bool getVisibility() const;

	void setVisibility(bool isVisible);

	bool hasItem(Item item) const;

private:
	bool isVisible;
	std::unordered_map<Item, ItemsGraphics> itemsData;

	void loadItems(ResourceManager& resourceManager, const std::string& fileName);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

struct ItemsGraphics : public sf::Drawable, public sf::Transformable
{
	ItemsGraphics() = default;
	ItemsGraphics(ResourceManager& resourceManager, const std::string& itemData);

	sf::Sprite sprite;
	std::string animationFile;
	sf::Text info;
	std::size_t quantity;
	std::size_t maxQuantity;
	thor::Animator<sf::Sprite, std::size_t> animator;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};