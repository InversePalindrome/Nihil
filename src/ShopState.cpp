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
#include <SFGUI/Image.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/ScrolledWindow.hpp>

#include <SFML/Graphics/Image.hpp>

#include <sstream>
#include <fstream>


ShopState::ShopState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	coinDisplay(stateData.resourceManager),
	backButton(sfg::Button::Create("BACK"))
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

	Parsers::parseGUIProperties(stateData.guiManager, "ShopGUI.txt");

	stateData.guiManager.addWidget(backButton);

	loadShopData("ShopData.txt");
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

void ShopState::loadShopData(const std::string& fileName)
{
	std::ifstream inFile(Path::miscellaneous / fileName);
	std::string line;

	auto notebook = sfg::Notebook::Create();

	const auto& inventory = this->stateData.games.front().getItems();

	std::unordered_map<std::string, sfg::Table::Ptr> itemCategories = 
	{ { "Characters", sfg::Table::Create() }, { "Weapons", sfg::Table::Create() } };

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string category;
		std::size_t itemID = 0u, imageID = 0u, price = 0u, row = 0u, column = 0u, rowSpan = 1u, columnSpan = 1u;
		float xPadding = 57.f, yPadding = 50.f;

		iStream >> category >> itemID >> imageID >> price >> row >> column;

		auto image = sfg::Image::Create(this->stateData.resourceManager.getImage(static_cast<ImagesID>(imageID)));
		
		itemCategories[category]->Attach(image, { column + 1, row * 2, columnSpan, rowSpan },
			0u, 0u, { xPadding, yPadding });

		auto item = static_cast<Item>(itemID);

		ShopData data(price);

		this->shopData.emplace(item, data);

		itemCategories[category]->Attach(this->shopData[item].itemButton, { column + 1, row * 2 + 1, columnSpan, rowSpan },
			0u, 0u, { xPadding, yPadding });

		this->loadButtonFunctions(itemID, static_cast<bool>(inventory.count(item)), price, this->shopData[item].itemButton);
	}

	for (const auto& category : itemCategories)
	{
		category.second->Attach(sfg::Label::Create(""), { 0u, 0u, 1u, 1u }, 0u, 0u, { 60.f, 50.f });

		auto window = sfg::ScrolledWindow::Create();

		auto scale = sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL);
		auto adjustment = sfg::Adjustment::Create(0.f, 0.f, 100.f, 1.f, 10.f, 10.f);

		window->SetRequisition(sf::Vector2f(1535.f, 969.f));
		window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_NEVER);
		window->SetVerticalAdjustment(adjustment);

		scale->SetAdjustment(adjustment);

		window->AddWithViewport(category.second);

		notebook->AppendPage(window, sfg::Label::Create("  " + category.first + "  "));
	}

	notebook->SetPosition(sf::Vector2f(250.f, 390.f));

	this->stateData.guiManager.addWidget(notebook);
}

void ShopState::loadButtonFunctions(std::size_t itemID, bool hasBeenPurchased, std::size_t price, sfg::Button::Ptr itemButton)
{
	itemButton->SetId(std::to_string(itemID));

	if (hasBeenPurchased)
	{
		itemButton->SetLabel("\t\tUse\t\t");
		
		itemButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]()
		{
			auto& game = stateData.games.front();

			

		});
	}
	else
	{
		itemButton->SetLabel("\t\t" + std::to_string(price) + "\t\t");

		itemButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this, itemButton, price]()
		{
			auto& game = stateData.games.front();

			if (game.getItems()[Item::Coin] >= price)
			{
				itemButton->SetLabel("\t\tUse\t\t");

				game.getItems()[Item::Coin] -= price;
				++game.getItems()[static_cast<Item>(std::stoull(itemButton->GetId()))];

				this->coinDisplay.setNumberOfCoins(game.getItems()[Item::Coin]);
			}
		});
	}
}

void ShopState::saveShopData(const std::string& fileName)
{
	std::ofstream outFile(Path::miscellaneous / fileName);
}

void ShopState::transitionToMenu()
{
	this->stateMachine.popState();

	auto pauseMenu = this->stateMachine[this->stateMachine.size() - 2].get();

	pauseMenu->showWidgets(true);
}

ShopData::ShopData(std::size_t price) :
	price(price),
	itemButton(sfg::Button::Create())
{
}