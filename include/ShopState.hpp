/*
Copyright (c) 2017 InversePalindrome
Nihil - ShopState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"
#include "Item.hpp"
#include "CoinDisplay.hpp"

#include <SFGUI/Button.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <unordered_map>


struct ShopData;

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

	std::unordered_map<Item, ShopData> shopData;

	void loadShopData(const std::string& fileName);
	void saveShopData(const std::string& fileName);

	void loadButtonFunctions(std::size_t itemID, bool hasBeenPurchased, std::size_t price, sfg::Button::Ptr itemButton);

	void transitionToMenu();
};

struct ShopData
{
	ShopData() = default;
	ShopData(std::size_t price);

	sfg::Button::Ptr itemButton;
	std::size_t price;
};