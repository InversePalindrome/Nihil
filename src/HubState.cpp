/*
Copyright (c) 2017 InversePalindrome
Nihil - HubState.cpp
InversePalindrome.com
*/


#include "HubState.hpp"
#include "StateMachine.hpp"
#include "SpriteParser.hpp"
#include "TextStyleParser.hpp"
#include "GUIParser.hpp"
#include "FilePaths.hpp"

#include <SFGUI/Label.hpp>
#include <SFGUI/RadioButton.hpp>

#include <cstdio>
#include <fstream>


HubState::HubState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	scrolledWindow(sfg::ScrolledWindow::Create()),
	nameEntry(sfg::Entry::Create()),
	backButton(sfg::Button::Create("BACK")),
	playButton(sfg::Button::Create("\t Play \t")),
	addButton(sfg::Button::Create("\t\tAdd\t\t")),
	deleteButton(sfg::Button::Create(" Delete ")),
	selectionBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.f)),
	gamePopupBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 70.f)),
	selectionButtons(sfg::RadioButtonGroup::Create()),
	isAddingGame(false)
{
	Parsers::parseSprite(stateData.resourceManager, "StateTitleBar.txt", titleBar);
	titleBar.setPosition({ 625.f, 80.f });

	Parsers::parseStyle(stateData.resourceManager, "StateTitle.txt", title);
	title.setString("Games");
	title.setPosition({ 780.f, 135.f });

	Parsers::parseSprite(stateData.resourceManager, "AddGamePanel.txt", addGameBackground);
	addGameBackground.setPosition({ 682.f, 510.f });

	Parsers::parseSprite(stateData.resourceManager, "AddGameTitleBar.txt", addGameTitleBar);
	addGameTitleBar.setPosition({766.f, 450.f});

	scrolledWindow->SetRequisition({ 800.f, 900.f });
	scrolledWindow->SetPosition({ 750.f, 320.f });
	scrolledWindow->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
	scrolledWindow->AddWithViewport(selectionBox);

	auto nameLabel = sfg::Label::Create("Name");
	auto doneButton = sfg::Button::Create("Done");

	nameEntry->SetRequisition({ 585.f, 0.f });
	doneButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { addGame(); });

	gamePopupBox->SetPosition({ 713.f, 480.f });
	gamePopupBox->Show(false);
	gamePopupBox->Pack(nameLabel);
	gamePopupBox->Pack(nameEntry);
	gamePopupBox->Pack(doneButton);

	backButton->SetPosition({ 12.f, 25.f });
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToMenu(); });

	addButton->SetPosition({ 445.f, 1300.f });
	addButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { showAddGamePopup(); });

	playButton->SetPosition({ 845.f, 1300.f });
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToPlay(); });

	deleteButton->SetPosition({ 1245.f, 1300.f });
	deleteButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { deleteGame(); });

	addGames();

	stateData.guiManager.addWidget(scrolledWindow);
	stateData.guiManager.addWidget(gamePopupBox);
	stateData.guiManager.addWidget(backButton);
	stateData.guiManager.addWidget(playButton);
	stateData.guiManager.addWidget(addButton);
	stateData.guiManager.addWidget(deleteButton);

	Parsers::parseGUIProperties(stateData.guiManager, "HubGUI.txt");
}

void HubState::handleEvent(const sf::Event& event)
{

}

void HubState::update(float deltaTime)
{

}

void HubState::draw()
{
	this->stateData.window.draw(this->titleBar);
	this->stateData.window.draw(this->title);

	if (this->isAddingGame)
	{
		this->stateData.window.draw(this->addGameBackground);
		this->stateData.window.draw(this->addGameTitleBar);
	}
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
	for (const auto& game : this->stateData.games)
	{
		auto gameButton = sfg::RadioButton::Create(game.getGameName(), this->selectionButtons);

		selectionBox->Pack(gameButton);
	}
}

void HubState::showAddGamePopup()
{
	this->isAddingGame = true;

	this->nameEntry->SetText("");
	this->selectionBox->Show(false);
	this->scrolledWindow->Show(false);
	this->gamePopupBox->Show(true);

	this->scrolledWindow->SetPosition({ 0.f, 0.f });
}

void HubState::addGame()
{
	if (!this->nameEntry->GetText().isEmpty() && this->nameEntry->GetText().toAnsiString().length() < 12u &&
		std::find_if(std::begin(this->stateData.games), std::end(this->stateData.games), [this](const auto& game)
	   { return game.getGameName() == this->nameEntry->GetText(); }) == std::end(this->stateData.games))
	{
		this->stateData.games.push_back(Game());
		this->stateData.games.back().setGameName(this->nameEntry->GetText());

		auto gameButton = sfg::RadioButton::Create(this->nameEntry->GetText(), this->selectionButtons);

		this->selectionBox->Pack(gameButton);

		this->saveGames("SavedGames.txt");
	}

	this->isAddingGame = false;
	this->gamePopupBox->Show(false);
	this->selectionBox->Show(true);
	this->scrolledWindow->Show(true);

	this->scrolledWindow->SetPosition({750.f, 320.f});
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
						std::remove(std::string(Path::blueprints / gameButton->_Get()->GetLabel() + "-Player.txt").c_str());
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
			
				this->stateData.soundManager.stopAllMusic();

				this->stateMachine.clearStates();
				this->stateMachine.pushState(StateID::Game);

				break;
			}
		}
	}
}