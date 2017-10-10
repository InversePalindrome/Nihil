/*
Copyright (c) 2017 InversePalindrome
Nihil - MenuState.cpp
InversePalindrome.com
*/


#include "MenuState.hpp"
#include "StateMachine.hpp"
#include "FilePaths.hpp"
#include "TextStyleParser.hpp"


MenuState::MenuState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	background(stateData.resourceManager.getTexture(TexturesID::MenuBackground)),
	playButton(sfg::Button::Create("\t\t\tPlay\t\t\t")),
	settingsButton(sfg::Button::Create("   Settings  ")),
	charactersButton(sfg::Button::Create("Characters"))
{
	stateData.window.setView(stateData.window.getDefaultView());

	background.setScale(stateData.window.getSize().x / background.getGlobalBounds().width, stateData.window.getSize().y / background.getGlobalBounds().height);

	Parsers::parseStyle(stateData.resourceManager, "TitleStyle.txt", titleLabel);
	titleLabel.setString("Nihil");
	titleLabel.setOrigin(titleLabel.getGlobalBounds().width / 2.f, titleLabel.getGlobalBounds().height / 2.f);
	titleLabel.setPosition(this->stateData.window.getSize().x / 2.f, 400.f);
	 
	playButton->SetPosition(sf::Vector2f(760.f, 820.f));
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToPlay(); });
	
	settingsButton->SetPosition(sf::Vector2f(760.f, 1000.f));
	settingsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToSettings(); });
	
	charactersButton->SetPosition(sf::Vector2f(760.f, 1180.f));
	charactersButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToCharacters(); });
	
	stateData.guiManager.setProperty("*", "FontName", Path::fonts / "8-BIT-WONDER.ttf");
	stateData.guiManager.setProperty("*", "Color", sf::Color(255u, 255u, 0u));
	stateData.guiManager.setProperty("*", "FontSize", 40.f);
	stateData.guiManager.setProperty("Button", "Padding", 25.f);
	stateData.guiManager.setProperty("Button", "BackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Button:PRELIGHT", "BackgroundColor", sf::Color(156u, 28u, 107u));
	stateData.guiManager.setProperty("Button:ACTIVE", "BackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Button:PRELIGHT", "Color", sf::Color(255u, 255u, 0u));
	stateData.guiManager.setProperty("Button:ACTIVE", "Color", sf::Color(255u, 255u, 0u));

	stateData.guiManager.addWidget(playButton);
	stateData.guiManager.addWidget(settingsButton);
	stateData.guiManager.addWidget(charactersButton);

	stateData.soundManager.playMusic("MenuDisco.wav", true);
}

void MenuState::handleEvent(const sf::Event& event)
{

}

void MenuState::update(float deltaTime)
{
}

void MenuState::draw()
{
	this->stateData.window.draw(this->background);
	this->stateData.window.draw(this->titleLabel);

	this->playButton->Show(true);
	this->settingsButton->Show(true);
	this->charactersButton->Show(true);
}

void MenuState::transitionToPlay()
{
	this->stateMachine.pushState(StateID::Hub);
}

void MenuState::transitionToSettings()
{
	this->stateMachine.pushState(StateID::Settings);
}

void MenuState::transitionToCharacters()
{
	this->stateMachine.pushState(StateID::Characters);
}