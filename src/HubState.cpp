/*
Copyright (c) 2017 InversePalindrome
Nihil - HubState.cpp
InversePalindrome.com
*/


#include "HubState.hpp"
#include "StateMachine.hpp"
#include "FilePaths.hpp"

#include <SFGUI/Label.hpp>
#include <SFGUI/RadioButton.hpp>

#include <cstdio>
#include <fstream>


HubState::HubState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	addGamePopup(sfg::Window::Create(sfg::Window::Style::NO_STYLE)),
	nameEntry(sfg::Entry::Create()),
	backButton(sfg::Button::Create("BACK")),
	playButton(sfg::Button::Create("\t Play \t")),
	addButton(sfg::Button::Create("\t\tAdd\t\t")),
	deleteButton(sfg::Button::Create(" Delete ")),
	selectionBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.f)),
	selectionButtons(sfg::RadioButtonGroup::Create())
{
	addGamePopup->SetPosition(sf::Vector2f(640.f, 400.f));
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

	backButton->SetPosition(sf::Vector2f(12.f, 25.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToMenu(); });

	addButton->SetPosition(sf::Vector2f(445.f, 1300.f));
	addButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { showAddGamePopup(); });

	playButton->SetPosition(sf::Vector2f(845.f, 1300.f));;
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToPlay(); });

	deleteButton->SetPosition(sf::Vector2f(1245.f, 1300.f));
	deleteButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { deleteGame(); });

	this->addGames();

	stateData.guiManager.setProperty("Entry", "FontSize", 50.f);
	stateData.guiManager.setProperty("Entry", "BackgroundColor", sf::Color(255, 255, 255u));
	stateData.guiManager.setProperty("Entry", "Color", sf::Color(0u, 0u, 0u));
	stateData.guiManager.setProperty("RadioButton", "FontSize", 60.f);
	stateData.guiManager.setProperty("RadioButton", "BoxSize", 30.f);
	stateData.guiManager.setProperty("RadioButton", "BackgroundColor", sf::Color(0u, 0u, 0u));
	stateData.guiManager.setProperty("RadioButton:PRELIGHT", "BackgroundColor", sf::Color(255u, 255u, 255u));
	stateData.guiManager.setProperty("RadioButton:ACTIVE", "BackgroundColor", sf::Color(255u, 255u, 255u));
	stateData.guiManager.setProperty("RadioButton:SELECTED", "BackgroundColor", sf::Color(255u, 255u, 255u));
	stateData.guiManager.setProperty("Window", "BackgroundColor", sf::Color(40u, 40u, 40u));
	stateData.guiManager.setProperty("Window", "Color", sf::Color(0u, 0u, 0u));

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
}

bool HubState::isTransparent() const
{
	return true;
}

bool HubState::isDependent() const
{
	return true;
}

void HubState::addGames()
{
	this->selectionBox->SetPosition(sf::Vector2f(750.f, 260.f));

	for (const auto& game : this->stateData.games)
	{
		auto gameButton = sfg::RadioButton::Create(game.getGameName(), this->selectionButtons);

		selectionBox->Pack(gameButton);
	}
	
	this->stateData.guiManager.addWidget(selectionBox);
}

void HubState::showAddGamePopup()
{
	const std::size_t maxNumberOfGames = 8u;

	if (this->selectionButtons->GetMembers().size() < maxNumberOfGames)
	{
		this->nameEntry->SetText("");
		this->selectionBox->Show(false);
		this->addGamePopup->Show(true);
	}
}

void HubState::addGame()
{
	if (!this->nameEntry->GetText().isEmpty() &&
		std::find_if(std::begin(this->stateData.games), std::end(this->stateData.games), [this](const auto& game)
	   { return game.getGameName() == this->nameEntry->GetText(); }) == std::end(this->stateData.games))
	{
		this->stateData.games.push_back(Game());
		this->stateData.games.back().setGameName(this->nameEntry->GetText());

		auto gameButton = sfg::RadioButton::Create(this->nameEntry->GetText(), this->selectionButtons);

		this->selectionBox->Pack(gameButton);

		this->saveGames("SavedGames.txt");
	}

	this->addGamePopup->Show(false);
	this->selectionBox->Show(true);
}

void HubState::deleteGame()
{
	for (auto& gameButton = std::begin(this->selectionButtons->GetMembers()); gameButton != std::end(this->selectionButtons->GetMembers());)
	{
		if (gameButton->_Get()->IsActive())
		{
			if (!this->stateData.games.empty())
			{
				auto gameIter = std::find_if(std::begin(this->stateData.games), std::end(this->stateData.games), [gameButton](const auto& game) { return game.getGameName() == gameButton->_Get()->GetLabel(); });

				if (gameIter != std::end(this->stateData.games))
				{
					for (const auto& level : gameIter->getLevels())
					{
						std::remove(std::string(Path::games / gameButton->_Get()->GetLabel() + '-' + level.name + ".txt").c_str());
					}

					this->stateData.games.erase(gameIter);

					gameButton->_Get()->Show(false);
					gameButton = this->selectionButtons->GetMembers().erase(gameButton);
				}
			}
		}
		else
		{
			++gameButton;
		}
	}

	this->saveGames("SavedGames.txt");
}

void HubState::saveGames(const std::string& fileName)
{
	std::ofstream outFile(Path::games / fileName);

	for (const auto& game : this->stateData.games)
	{
		outFile << game << '\n';
	}
}

void HubState::transitionToMenu()
{
	this->stateMachine.popState();

	auto menu = this->stateMachine[this->stateMachine.size() - 2].get();

	menu->showWidgets(true);
}

void HubState::transitionToPlay()
{
	for (auto& gameButton : this->selectionButtons->GetMembers())
	{
		if (gameButton._Get()->IsActive())
		{
			auto gameIter = std::find_if(std::begin(this->stateData.games), std::end(this->stateData.games), [gameButton](const auto& game) { return game.getGameName() == gameButton._Get()->GetLabel(); });

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