/*
Copyright (c) 2017 InversePalindrome
Nihil - CharactersState.cpp
InversePalindrome.com
*/


#include "CharactersState.hpp"
#include "StateMachine.hpp"

#include <SFGUI/Image.hpp>

#include <sstream>
#include <fstream>


CharactersState::CharactersState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	background(stateData.resourceManager.getTexture(TexturesID::MenuBackground)),
	coinDisplay(stateData.resourceManager),
	backButton(sfg::Button::Create("BACK")),
	itemsTable(sfg::Table::Create()),
	scrollbarScale(sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL)),
	scrollbarAdjustment(sfg::Adjustment::Create(0.f, 0.f, 100.f, 1.f, 10.f, 10.f)),
	scrolledWindow(sfg::ScrolledWindow::Create()),
	characterButtons(sfg::RadioButtonGroup::Create())
{
	loadCharacters("Resources/Files/Characters.txt");

	background.setScale(stateData.window.getSize().x / background.getGlobalBounds().width, stateData.window.getSize().y / background.getGlobalBounds().height);

	coinDisplay.setPosition(1600.f, 120.f);
	coinDisplay.setNumberOfCoins(stateData.games.front().getCoins());

	backButton->SetPosition(sf::Vector2f(12.f, 65.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	scrolledWindow->SetPosition(sf::Vector2f(125.f, 400.f));
	scrolledWindow->SetRequisition(sf::Vector2f(1800.f, 650.f));
	scrolledWindow->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_NEVER);
	
	scrolledWindow->SetHorizontalAdjustment(scrollbarAdjustment);
	
	itemsTable->SetRequisition(sf::Vector2f(scrolledWindow->GetRequisition().x, scrolledWindow->GetRequisition().y));
	scrolledWindow->AddWithViewport(itemsTable);
	
    scrollbarScale->SetAdjustment(scrollbarAdjustment);

	for (auto& characterButton : characterButtons->GetMembers())
	{
		characterButton._Get()->SetState(sfg::Widget::State::SELECTED);
		characterButton._Get()->SetState(sfg::Widget::State::NORMAL);

		if (stateData.games.front().getCharacterName() == characterButton._Get()->GetId())
		{
			characterButton._Get()->SetActive(true);
		}
	}

	stateData.guiManager.setProperty("ScrolledWindow", "BorderWidth", 0.f);
	stateData.guiManager.setProperty("ScrolledWindow", "ScrollbarSpacing", 20.f);
	stateData.guiManager.setProperty("Scrollbar", "TroughColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Scrollbar", "SliderColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("Scrollbar", "BorderColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Scrollbar", "StepperBackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Scrollbar", "StepperArrowColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton", "BackgroundColor", sf::Color(102u, 0u, 204u));
	stateData.guiManager.setProperty("RadioButton", "BoxSize", 30.f);
	stateData.guiManager.setProperty("RadioButton:PRELIGHT", "BackgroundColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton:ACTIVE", "BackgroundColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton:SELECTED", "BackgroundColor", sf::Color(15u, 192u, 252u));

	stateData.guiManager.addWidget(backButton);
	stateData.guiManager.addWidget(scrolledWindow);
}

void CharactersState::handleEvent(const sf::Event& event)
{

}

void CharactersState::update(float deltaTime)
{
	this->coinDisplay.update(deltaTime);
}

void CharactersState::draw()
{
	this->stateData.window.draw(this->background);
	this->stateData.window.draw(this->coinDisplay);
}

void CharactersState::loadCharacters(const std::string& filePath)
{
	this->charactersFile = filePath;

	std::ifstream inFile(filePath);
	std::string line;

	std::size_t columnIndex = 0u, rowIndex = 0u, columnSpan = 1u, rowSpan = 1u;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string characterID;
		std::size_t imageID = 0, price = 0u;
		bool lockStatus;
		
		iStream >> characterID >> imageID >> price >> lockStatus;

		this->charactersData.emplace(characterID, CharacterData(characterID, imageID, price, lockStatus));
		
		auto characterButton = sfg::RadioButton::Create("", this->characterButtons);
		
		characterButton->SetId(characterID);
		characterButton->SetImage(sfg::Image::Create(this->stateData.resourceManager.getImage(static_cast<ImagesID>(imageID))));
	
		this->itemsTable->Attach(characterButton, sf::Rect<sf::Uint32>(columnIndex, rowIndex, columnSpan, rowSpan),
			sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND, sf::Vector2f(100.f, 0.f));

		++rowIndex;

		auto purchaseButton = sfg::Button::Create("\t\t" + std::to_string(price) + "\t\t");
	
		this->itemsTable->Attach(purchaseButton, sf::Rect<sf::Uint32>(columnIndex, rowIndex, columnSpan, rowSpan),
			0u, 0u, sf::Vector2f(100.f, 0.f));

		if (lockStatus)
		{
			purchaseButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this, characterButton, purchaseButton]() { this->purchasedCharacter(characterButton, purchaseButton); });
		}
		else
		{
			purchaseButton->Show(false);
		}

		characterButton->GetSignal(sfg::ToggleButton::OnToggle).Connect([this, characterButton, purchaseButton]()
		{
			if (characterButton->IsActive() && !purchaseButton->IsGloballyVisible())
			{
				selectedCharacter(characterButton->GetId());
			}
			else
			{
				characterButton->SetActive(false);
			}
		});

		++columnIndex;
		rowIndex = 0u;
	}
}

void CharactersState::selectedCharacter(const std::string& character)
{
	this->stateData.games.front().setCharacterName(character);
}

void CharactersState::purchasedCharacter(sfg::RadioButton::Ptr characterButton, sfg::Button::Ptr purchaseButton)
{
	const auto& price = std::stoull(purchaseButton->GetLabel().toAnsiString());

	if (this->stateData.games.front().getCoins() >= price)
	{
		this->stateData.games.front().setCoins(this->stateData.games.front().getCoins() - price);
		this->coinDisplay.setNumberOfCoins(this->stateData.games.front().getCoins());

		purchaseButton->Show(false);

		this->charactersData[characterButton->GetId()].lockStatus = false;

		this->saveCharactersData();
	}
}

void CharactersState::saveCharactersData()
{
	std::ofstream outFile(this->charactersFile);

	for (const auto& characterData : this->charactersData)
	{
		outFile << characterData.second.characterName << ' ' << characterData.second.imageID << ' '  << characterData.second.price << ' ' << characterData.second.lockStatus << '\n';
	}
}

void CharactersState::transitionToMenu()
{
	this->stateMachine.popState();
}

CharacterData::CharacterData() :
	CharacterData("", 0u, 0u, false)
{
}

CharacterData::CharacterData(const std::string& characterName, std::size_t imageID, std::size_t price, bool lockStatus) :
	characterName(characterName),
	imageID(imageID),
	price(price),
	lockStatus(lockStatus)
{
}