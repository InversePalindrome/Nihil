/*
Copyright (c) 2017 InversePalindrome
Nihil - ItemsDisplay.hpp
InversePalindrome.com
*/


#pragma once

#include "Item.hpp"
#include "Renderable.hpp"
#include "Animation.hpp"
#include "ResourceManager.hpp"

#include <Thor/Animations/Animator.hpp>
#include <Thor/Animations/FrameAnimation.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <string>
#include <unordered_map>


struct ItemData;

class ItemsDisplay : public Renderable
{
public:
	ItemsDisplay(ResourceManager& resourceManager);

	ItemData& operator[](Item item);

	void update(float deltaTime);

	bool getVisibility() const;

	void setVisibility(bool isVisible);

	bool hasItem(Item item) const;

private:
	bool isVisible;
	std::unordered_map<Item, ItemData> itemsData;

	void loadItems(ResourceManager& resourceManager, const std::string& fileName);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

struct ItemData : public sf::Drawable, public sf::Transformable
{
	ItemData() = default;
	ItemData(ResourceManager& resourceManager, const std::string& itemData);

	sf::Sprite sprite;
	std::string animationFile;
	sf::Text info;
	std::size_t quantity;
	std::size_t maxQuantity;
	Animator animator;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};