/*
Copyright (c) 2017 InversePalindrome
Nihil - HubState.cpp
InversePalindrome.com
*/


#include "HubState.hpp"
#include "StateMachine.hpp"

#include <SFGUI/Box.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/RadioButton.hpp>

#include <fstream>
#include <sstream>


HubState::HubState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	addGamePopup(sfg::Window::Create(sf::Style::Titlebar | sf::Style::Close)),
	nameEntry(sfg::Entry::Create()),
	backButton(sfg::Button::Create("BACK")),
	playButton(sfg::Button::Create("\t Play \t")),
	addButton(sfg::Button::Create("\t\tAdd\t\t")),
	deleteButton(sfg::Button::Create(" Delete ")),
	selectionButtons(sfg::RadioButtonGroup::Create())
{
	addGamePopup->SetTitle("Add Game");
	addGamePopup->SetPosition(sf::Vector2f(750.f, 400.f));
	addGamePopup->Show(false);

	auto gamePopupBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);

	auto nameLabel = sfg::Label::Create("Name");
	auto doneButton = sfg::Button::Create("Done");

	nameEntry->SetRequisition(sf::Vector2f(600.f, 0.f));

	doneButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { addGame(); });

	gamePopupBox->Pack(nameLabel);
	gamePopupBox->Pack(nameEntry);
	gamePopupBox->Pack(doneButton);

	addGamePopup->Add(gamePopupBox);

	backButton->SetPosition(sf::Vector2f(12.f, 65.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToMenu(); });

	addButton->SetPosition(sf::Vector2f(475.f, 1300.f));
	addButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { showAddGamePopup(); });

	playButton->SetPosition(sf::Vector2f(875.f, 1300.f));;
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToPlay(); });

	deleteButton->SetPosition(sf::Vector2f(1275.f, 1300.f));
	deleteButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { deleteGame(); });

	this->loadGames("Resources/Files/SavedGames.txt");

	stateData.guiManager.setProperty("Entry", "FontSize", 60.f);
	stateData.guiManager.setProperty("RadioButton", "FontSize", 80.f);
	stateData.guiManager.setProperty("Window", "BackgroundColor", sf::Color(75u, 0u, 130u));

	this->stateData.guiManager.addWidget(addGamePopup);
	this->stateData.guiManager.addWidget(backButton);
	this->stateData.guiManager.addWidget(playButton);
	this->stateData.guiManager.addWidget(addButton);
	this->stateData.guiManager.addWidget(deleteButton);
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

void HubState::loadGames(const std::string& pathFile)
{
	std::ifstream inFile(pathFile);
	std::string line;

	auto selectionBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 30.f);
	selectionBox->SetPosition(sf::Vector2f(750.f, 400.f));

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string gameName;

		iStream >> gameName;
		
		auto gameButton = sfg::RadioButton::Create(gameName, this->selectionButtons);

		selectionBox->Pack(gameButton);
	}

	this->stateData.guiManager.addWidget(selectionBox);
}

void HubState::showAddGamePopup()
{
	this->addGamePopup->Show(true);
}

void HubState::addGame()
{
	this->addGamePopup->Show(false);
}

void HubState::deleteGame()
{

}

void HubState::transitionToMenu()
{
	this->stateMachine.popState();
}

void HubState::transitionToPlay()
{
	this->stateMachine.clearStates();
	this->stateMachine.pushState(StateID::Game);
}