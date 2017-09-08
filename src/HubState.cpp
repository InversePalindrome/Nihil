/*
Copyright (c) 2017 InversePalindrome
Nihil - HubState.cpp
InversePalindrome.com
*/


#include "HubState.hpp"
#include "StateMachine.hpp"

#include <SFGUI/Label.hpp>
#include <SFGUI/RadioButton.hpp>

#include <fstream>


HubState::HubState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	addGamePopup(sfg::Window::Create(sfg::Window::Style::TITLEBAR | sfg::Window::Style::TOPLEVEL)),
	nameEntry(sfg::Entry::Create()),
	backButton(sfg::Button::Create("BACK")),
	playButton(sfg::Button::Create("\t Play \t")),
	addButton(sfg::Button::Create("\t\tAdd\t\t")),
	deleteButton(sfg::Button::Create(" Delete ")),
	selectionBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.f)),
	selectionButtons(sfg::RadioButtonGroup::Create())
{
	addGamePopup->SetTitle("Add Game");
	addGamePopup->SetPosition(sf::Vector2f(675.f, 400.f));
	addGamePopup->Show(false);
	
	auto gamePopupBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 70.f);

	auto nameLabel = sfg::Label::Create("Name");
	auto doneButton = sfg::Button::Create("Done");

	nameEntry->SetRequisition(sf::Vector2f(700.f, 0.f));

	doneButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { addGame(); });

	gamePopupBox->Pack(nameLabel);
	gamePopupBox->Pack(nameEntry);
	gamePopupBox->Pack(doneButton);

	addGamePopup->Add(gamePopupBox);

	backButton->SetPosition(sf::Vector2f(12.f, 65.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToMenu(); });

	addButton->SetPosition(sf::Vector2f(450.f, 1300.f));
	addButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { showAddGamePopup(); });

	playButton->SetPosition(sf::Vector2f(850.f, 1300.f));;
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToPlay(); });

	deleteButton->SetPosition(sf::Vector2f(1250.f, 1300.f));
	deleteButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { deleteGame(); });

	this->addGames();

	stateData.guiManager.setProperty("Entry", "FontSize", 50.f);
	stateData.guiManager.setProperty("RadioButton", "FontSize", 60.f);
	stateData.guiManager.setProperty("RadioButton", "BoxSize", 30.f);
	stateData.guiManager.setProperty("RadioButton", "BackgroundColor", sf::Color(102u, 0u, 204u));
	stateData.guiManager.setProperty("RadioButton:PRELIGHT", "BackgroundColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton:ACTIVE", "BackgroundColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton:SELECTED", "BackgroundColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("Window", "BackgroundColor", sf::Color(25u, 25u, 112u));
	stateData.guiManager.setProperty("Window", "Color", sf::Color(70u, 173u, 212u));
	stateData.guiManager.setProperty("Entry", "BackgroundColor", sf::Color(75u, 0u, 130u));

	stateData.guiManager.addWidget(addGamePopup);
	stateData.guiManager.addWidget(backButton);
	stateData.guiManager.addWidget(playButton);
	stateData.guiManager.addWidget(addButton);
	stateData.guiManager.addWidget(deleteButton);
}

void HubState::handleEvent(const sf::Event& event)
{

}

void HubState::update(float deltaTime)
{

}

void HubState::draw()
{
	this->stateData.window.draw(this->background);
}

void HubState::addGames()
{
	this->selectionBox->SetPosition(sf::Vector2f(750.f, 350.f));

	for (const auto& game : this->stateData.games)
	{
		auto gameButton = sfg::RadioButton::Create(game.getName(), this->selectionButtons);

		selectionBox->Pack(gameButton);
	}
	
	this->stateData.guiManager.addWidget(selectionBox);
}

void HubState::showAddGamePopup()
{
	this->nameEntry->SetText("");
	this->selectionBox->Show(false);
	this->addGamePopup->Show(true);
}

void HubState::addGame()
{
	this->stateData.games.push_back(Game(nameEntry->GetText()));

	auto gameButton = sfg::RadioButton::Create(this->nameEntry->GetText(), this->selectionButtons);

	this->selectionBox->Pack(gameButton);

	this->addGamePopup->Show(false);
	this->selectionBox->Show(true);

	this->saveGames("Resources/Files/SavedGames.txt");
}

void HubState::deleteGame()
{
	for (auto& gameButton : this->selectionButtons->GetMembers())
	{
		if (gameButton._Get()->IsActive())
		{
			gameButton._Get()->Show(false);

			if (!this->stateData.games.empty())
			{
				this->stateData.games.erase(std::remove_if(std::begin(this->stateData.games), std::end(this->stateData.games), [gameButton](const auto& game) { return game.getName() == gameButton._Get()->GetLabel(); }));
			}
		}
	}

	this->saveGames("Resources/Files/SavedGames.txt");
}

void HubState::saveGames(const std::string& pathFile)
{
	std::ofstream outFile(pathFile);

	for (const auto& game : this->stateData.games)
	{
		outFile << game << '\n';
	}
}

void HubState::transitionToMenu()
{
	this->stateMachine.popState();
}

void HubState::transitionToPlay()
{
	for (auto& gameButton : this->selectionButtons->GetMembers())
	{
		if (gameButton._Get()->IsActive())
		{
			auto gameIter = std::find_if(std::begin(this->stateData.games), std::end(this->stateData.games), [gameButton](const auto& game) { return game.getName() == gameButton._Get()->GetLabel(); });

			if (!this->stateData.games.empty() && gameIter != std::end(this->stateData.games))
			{
				std::iter_swap(gameIter, std::begin(this->stateData.games));

				this->stateMachine.clearStates();
				this->stateMachine.pushState(StateID::Game);

				break;
			}
		}
	}
}