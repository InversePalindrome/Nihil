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
	playButton(sfg::Button::Create("       Play       ")),
	settingsButton(sfg::Button::Create("    Settings   ")),
	quitButton(sfg::Button::Create("       Quit        ")),
	isTitleVisible(true)
{
	stateData.window.setView(stateData.window.getDefaultView());

	background.setScale(stateData.window.getSize().x / background.getGlobalBounds().width, stateData.window.getSize().y / background.getGlobalBounds().height);

	Parsers::parseStyle(stateData.resourceManager, "TitleStyle.txt", titleLabel);
	titleLabel.setString("Nihil");
	titleLabel.setOrigin(titleLabel.getGlobalBounds().width / 2.f, titleLabel.getGlobalBounds().height / 2.f);
	titleLabel.setPosition(this->stateData.window.getSize().x / 2.f, 400.f);
	 
	playButton->SetPosition({ 740.f, 820.f });
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToState(StateID::Hub); });
	
	settingsButton->SetPosition({ 740.f, 1000.f });
	settingsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToState(StateID::Settings); });

	quitButton->SetPosition({ 740.f, 1180.f });
	quitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&stateData] { stateData.window.close(); });
	
	Parsers::parseGUIProperties(stateData.guiManager, "MenuGUI.txt");

	stateData.guiManager.addWidget(playButton);
	stateData.guiManager.addWidget(settingsButton);
	stateData.guiManager.addWidget(quitButton);

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

	if (this->isTitleVisible)
	{
		this->stateData.window.draw(this->titleLabel);
	}
}

void MenuState::showWidgets(bool showStatus) 
{
	this->playButton->Show(showStatus);
	this->settingsButton->Show(showStatus);
	this->quitButton->Show(showStatus);

	this->isTitleVisible = showStatus;
}

void MenuState::transitionToState(StateID stateID)
{
	this->isTitleVisible = false;
	this->stateMachine.pushState(stateID);
}