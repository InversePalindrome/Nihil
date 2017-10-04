/*
Copyright (c) 2017 InversePalindrome
Nihil - CharactersState.cpp
InversePalindrome.com
*/


#include "CharactersState.hpp"
#include "StateMachine.hpp"
#include "FilePaths.hpp"

#include <SFGUI/Image.hpp>

#include <sstream>
#include <fstream>


CharactersState::CharactersState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	background(stateData.resourceManager.getTexture(TexturesID::MenuBackground)),
	coinDisplay(stateData.resourceManager),
	backButton(sfg::Button::Create("BACK")),
	gameChoices(sfg::ComboBox::Create()),
	itemsTable(sfg::Table::Create()),
	scrollbarScale(sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL)),
	scrollbarAdjustment(sfg::Adjustment::Create(0.f, 0.f, 100.f, 1.f, 10.f, 10.f)),
	scrolledWindow(sfg::ScrolledWindow::Create()),
	characterButtons(sfg::RadioButtonGroup::Create())
{
	background.setScale(stateData.window.getSize().x / background.getGlobalBounds().width, stateData.window.getSize().y / background.getGlobalBounds().height);

	coinDisplay.setPosition(1600.f, 120.f);

	backButton->SetPosition(sf::Vector2f(12.f, 65.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	gameChoices->SetPosition(sf::Vector2f(775.f, 275.f));
	gameChoices->SetRequisition(sf::Vector2f(400.f, 0.f));

	for (const auto& game : this->stateData.games)
	{
		gameChoices->AppendItem(game.getGameName());
	}

	gameChoices->GetSignal(sfg::ComboBox::OnSelect).Connect([this]() { selectGame(); });

	if (!stateData.games.empty())
	{
		coinDisplay.setNumberOfCoins(stateData.games.front().getItems()[Item::Coin]);
		gameChoices->SelectItem(0u);
	}

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

		if (!stateData.games.empty() && stateData.games.front().getCurrentCharacter() == characterButton._Get()->GetId())
		{
			characterButton._Get()->SetActive(true);
		}
	}

	stateData.guiManager.setProperty("ComboBox:PRELIGHT", "BackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("ComboBox:NORMAL", "BackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("ComboBox:ACTIVE", "BackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("ComboBox:SELECTED", "BackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("ComboBox", "Color", sf::Color(255.f, 255.f, 0.f));
	stateData.guiManager.setProperty("ComboBox", "ArrowColor", sf::Color(15u, 192u, 252u));
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
	stateData.guiManager.addWidget(gameChoices);
	stateData.guiManager.addWidget(scrolledWindow);

	loadCharacters("Characters.txt");
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
	std::ifstream inFile(Path::miscellaneous / filePath);
	std::string line;

	std::size_t columnIndex = 0u, rowIndex = 0u, columnSpan = 1u, rowSpan = 1u;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string characterID;
		std::size_t imageID = 0, price = 0u;
		
		iStream >> characterID >> imageID >> price;

		this->charactersData.emplace(characterID, CharacterData(characterID, imageID, price));
		
		auto characterButton = sfg::RadioButton::Create("", this->characterButtons);
		
		characterButton->SetId(characterID);
		characterButton->SetImage(sfg::Image::Create(this->stateData.resourceManager.getImage(static_cast<ImagesID>(imageID))));
	
		this->itemsTable->Attach(characterButton, sf::Rect<sf::Uint32>(columnIndex, rowIndex, columnSpan, rowSpan),
			sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND, sf::Vector2f(100.f, 0.f));

		++rowIndex;

		auto purchaseButton = sfg::Button::Create("\t\t" + std::to_string(price) + "\t\t");
	
		this->itemsTable->Attach(purchaseButton, sf::Rect<sf::Uint32>(columnIndex, rowIndex, columnSpan, rowSpan),
			0u, 0u, sf::Vector2f(100.f, 0.f));

		if (!this->stateData.games.empty() && !this->stateData.games.front().getCharacters().get<1>().find(characterID)->isLoaded)
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
	if (!this->stateData.games.empty())
	{
		this->stateData.games.front().setCurrentCharacter(character);
	}
}

void CharactersState::purchasedCharacter(sfg::RadioButton::Ptr characterButton, sfg::Button::Ptr purchaseButton)
{
	const auto& price = std::stoull(purchaseButton->GetLabel().toAnsiString());

	if (!this->stateData.games.empty() && this->stateData.games.front().getItems()[Item::Coin] >= price)
	{
		this->stateData.games.front().getItems()[Item::Coin] = (this->stateData.games.front().getItems()[Item::Coin] - price);

		this->coinDisplay.setNumberOfCoins(this->stateData.games.front().getItems()[Item::Coin]);

		purchaseButton->Show(false);

		this->stateData.games.front().getCharacters().get<1>().modify(this->stateData.games.front().getCharacters().get<1>().find(characterButton->GetId()), [](auto& character) { character.isLoaded = false; });

		std::ofstream outFile(Path::games / "SavedGames.txt");

		for (const auto& game : this->stateData.games)
		{
			outFile << game << '\n';
		}
	}
}

void CharactersState::selectGame()
{
	if (this->stateData.games.size() > 1u)
	{
		auto selectedGame = std::find_if(std::begin(this->stateData.games), std::end(this->stateData.games), [this](const auto& game) { return game.getGameName() == this->gameChoices->GetSelectedText(); });

		std::iter_swap(std::begin(this->stateData.games), selectedGame);
		this->coinDisplay.setNumberOfCoins(this->stateData.games.front().getItems()[Item::Coin]);

		for (const auto& widget : this->itemsTable->GetChildren())
		{
			widget->Show(false);
		}

		this->loadCharacters("Characters.txt");
	}
}

void CharactersState::transitionToMenu()
{
	this->stateMachine.popState();
}

CharacterData::CharacterData() :
	CharacterData("", 0u, 0u)
{
}

CharacterData::CharacterData(const std::string& characterName, std::size_t imageID, std::size_t price) :
	characterName(characterName),
	imageID(imageID),
	price(price)
{
}