/*
Copyright (c) 2017 InversePalindrome
Nihil - MenuState.cpp
InversePalindrome.com
*/


#include "MenuState.hpp"
#include "StateMachine.hpp"
#include "FilePaths.hpp"
#include "GUIParser.hpp"
#include "EffectParser.hpp"
#include "TextStyleParser.hpp"

#include <Thor/Particles/Emitters.hpp>


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
	
	Parsers::parseGUIProperties(stateData.guiManager, "MenuGUI.txt");

	stateData.guiManager.addWidget(playButton);
	stateData.guiManager.addWidget(settingsButton);
	stateData.guiManager.addWidget(charactersButton);

	stateData.soundManager.playMusic("MenuDisco.wav", true);

	Parsers::parseParticleSystem(stateData.resourceManager, "MenuParticles.txt", particleSystem);
	particleSystem.addEmitter(Parsers::parseEmitter("MenuEmitters.txt"));
}

void MenuState::handleEvent(const sf::Event& event)
{

}

void MenuState::update(float deltaTime)
{
	this->particleSystem.update(sf::seconds(deltaTime));
}

void MenuState::draw()
{
	this->stateData.window.draw(this->background);
	this->stateData.window.draw(this->particleSystem);
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