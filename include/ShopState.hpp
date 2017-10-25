/*
Copyright (c) 2017 InversePalindrome
Nihil - ShopState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"
#include "CoinDisplay.hpp"

#include <SFGUI/Scale.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/ScrolledWindow.hpp>

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
	sfg::Scale::Ptr scrollbarScale;
	sfg::Adjustment::Ptr scrollbarAdjustment;
	sfg::Notebook::Ptr itemCategories;
	sfg::ScrolledWindow::Ptr scrolledWindow;

	std::unordered_map<std::string, ShopData> graphicsData;

	sfg::Table::Ptr loadItems(const std::string& fileName);

	void transitionToMenu();
};

struct ShopData
{
	ShopData();
	ShopData(const std::string& itemName, std::size_t price);

	std::string itemName;
	std::size_t price;
};