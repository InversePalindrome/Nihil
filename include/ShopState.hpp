/*
Copyright (c) 2017 InversePalindrome
Nihil - ShopState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"
#include "Item.hpp"
#include "ItemCategory.hpp"
#include "CoinDisplay.hpp"

#include <SFGUI/Button.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <any>
#include <unordered_map>


struct ShopGraphics;

class ShopState : public State
{
public:
	ShopState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

	virtual bool isTransparent() const override;

private:
	sf::Sprite background;
	sf::Sprite titleBar;
	sf::Text title;

	CoinDisplay coinDisplay;

	sfg::Button::Ptr backButton;

	std::unordered_map<Item, ShopGraphics> shopGraphics;
	std::unordered_map<Item, std::any> shopData;
	std::unordered_map<ItemCategory, std::string> categoryNames;

	void loadShopData(const std::string& fileName);

	void loadButtonFunctions(Item item, ItemCategory itemCategory, bool hasBeenPurchased, std::size_t price, sfg::Button::Ptr itemButton);

	void transitionToMenu();
};

struct ShopGraphics
{
	ShopGraphics() = default;
	ShopGraphics(ItemCategory category, std::size_t price);

	sfg::Button::Ptr itemButton;
	ItemCategory category;
	std::size_t price;
};

struct CharacterData
{
	CharacterData(const std::string& spriteFile, const std::string& animationsFile);

	std::string spriteFile;
	std::string animationsFile;
};

struct WeaponData
{
	WeaponData(const std::string& weaponID, float reloadTime);

	std::string weaponID;
	float reloadTime;
};