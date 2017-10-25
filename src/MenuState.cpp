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
	playButton(sfg::Button::Create("\t\t\t\tPlay\t\t\t\t")),
	settingsButton(sfg::Button::Create("   \tSettings\t  ")),
	achievementsButton(sfg::Button::Create("Achievements")),
	isTitleVisible(true)
{
	stateData.window.setView(stateData.window.getDefaultView());

	background.setScale(stateData.window.getSize().x / background.getGlobalBounds().width, stateData.window.getSize().y / background.getGlobalBounds().height);

	Parsers::parseStyle(stateData.resourceManager, "TitleStyle.txt", titleLabel);
	titleLabel.setString("Nihil");
	titleLabel.setOrigin(titleLabel.getGlobalBounds().width / 2.f, titleLabel.getGlobalBounds().height / 2.f);
	titleLabel.setPosition(this->stateData.window.getSize().x / 2.f, 400.f);
	 
	playButton->SetPosition(sf::Vector2f(730.f, 820.f));
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToState(StateID::Hub); });
	
	settingsButton->SetPosition(sf::Vector2f(730.f, 1000.f));
	settingsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToState(StateID::Settings); });

    achievementsButton->SetPosition(sf::Vector2f(730.f, 1180.f));
	achievementsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToState(StateID::Achievements); });
	
	Parsers::parseGUIProperties(stateData.guiManager, "MenuGUI.txt");

	stateData.guiManager.addWidget(playButton);
	stateData.guiManager.addWidget(settingsButton);
	stateData.guiManager.addWidget(achievementsButton);

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
	this->achievementsButton->Show(showStatus);

	this->isTitleVisible = showStatus;
}

void MenuState::transitionToState(StateID stateID)
{
	this->isTitleVisible = false;
	this->stateMachine.pushState(stateID);
}