/*
Copyright (c) 2017 InversePalindrome
Nihil - ShopState.cpp
InversePalindrome.com
*/


#include "ShopState.hpp"
#include "StateMachine.hpp"
#include "GUIParser.hpp"
#include "FilePaths.hpp"
#include "SpriteParser.hpp"
#include "TextStyleParser.hpp"

#include <SFGUI/Label.hpp>

#include <sstream>
#include <fstream>


ShopState::ShopState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	coinDisplay(stateData.resourceManager),
	backButton(sfg::Button::Create("BACK")),
	scrollbarScale(sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL)),
	scrollbarAdjustment(sfg::Adjustment::Create(0.f, 0.f, 100.f, 1.f, 10.f, 10.f)),
	itemCategories(sfg::Notebook::Create()),
	scrolledWindow(sfg::ScrolledWindow::Create())
{
	coinDisplay.setPosition(1600.f, 60.f);

	Parsers::parseSprite(stateData.resourceManager, "LargePanel.txt", background);
	background.setOrigin(background.getGlobalBounds().width / 2.f, background.getGlobalBounds().height / 2.f);
	background.setPosition(stateData.window.getDefaultView().getCenter().x - 160.f, 800.f);

	Parsers::parseSprite(stateData.resourceManager, "StateTitleBar.txt", titleBar);
	titleBar.setOrigin(titleBar.getLocalBounds().width / 2.f, titleBar.getLocalBounds().height / 2.f);
	titleBar.setPosition(stateData.window.getDefaultView().getCenter().x, 250.f);

	Parsers::parseStyle(stateData.resourceManager, "StateTitle.txt", title);
	title.setString("Shop");
	title.setOrigin(title.getLocalBounds().width / 2.f, title.getLocalBounds().height / 2.f);
	title.setPosition(stateData.window.getDefaultView().getCenter().x, 242.f);

	backButton->SetPosition(sf::Vector2f(12.f, 25.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	if (!stateData.games.empty())
	{
		coinDisplay.setNumberOfCoins(stateData.games.front().getItems()[Item::Coin]);
	}

	itemCategories->AppendPage(loadItems("CharacterItems.txt"), sfg::Label::Create("  Characters  "));
	itemCategories->AppendPage(loadItems("WeaponItems.txt"), sfg::Label::Create("  Weapons  "));

	scrolledWindow->SetPosition(sf::Vector2f(250.f, 390.f));
	scrolledWindow->SetRequisition(sf::Vector2f(1600.f, 1050.f));
	scrolledWindow->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_NEVER);
	
	scrolledWindow->SetHorizontalAdjustment(scrollbarAdjustment);
	
	scrolledWindow->AddWithViewport(itemCategories);
	
    scrollbarScale->SetAdjustment(scrollbarAdjustment);

	Parsers::parseGUIProperties(stateData.guiManager, "ShopGUI.txt");

	stateData.guiManager.addWidget(backButton);
	stateData.guiManager.addWidget(scrolledWindow);
}

void ShopState::handleEvent(const sf::Event& event)
{

}

void ShopState::update(float deltaTime)
{
	this->coinDisplay.update(deltaTime);
}

void ShopState::draw()
{
	this->stateData.window.draw(this->coinDisplay);
	this->stateData.window.draw(this->background);
	this->stateData.window.draw(this->titleBar);
	this->stateData.window.draw(this->title);
}

bool ShopState::isTransparent() const
{
	return true;
}

sfg::Table::Ptr ShopState::loadItems(const std::string& fileName)
{
	std::ifstream inFile(Path::miscellaneous / fileName);
	std::string line;

	while (std::getline(inFile, line))
	{

	}

	auto items = sfg::Table::Create();

	return items;
}

void ShopState::transitionToMenu()
{
	this->stateMachine.popState();

	auto pauseMenu = this->stateMachine[this->stateMachine.size() - 2].get();

	pauseMenu->showWidgets(true);
}

ShopData::ShopData() :
	ShopData("", 0u)
{
}

ShopData::ShopData(const std::string& itemName, std::size_t price) :
	itemName(itemName),
	price(price)
{
}