/*
Copyright (c) 2017 InversePalindrome
Nihil - SettingsState.cpp
InversePalindrome.com
*/


#include "SettingsState.hpp"
#include "StateMachine.hpp"

#include <SFGUI/Image.hpp>


SettingsState::SettingsState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	backButton(sfg::Button::Create())
{
	backButton->SetImage(sfg::Image::Create(stateData.resourceManager.getImage(ImagesID::BackButton)));
	backButton->SetPosition(sf::Vector2f(12.f, 65.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	stateData.guiManager.addWidget(backButton);
}

void SettingsState::handleEvent(const sf::Event& event)
{

}

void SettingsState::update(float deltaTime)
{
	
}

void SettingsState::draw()
{
	
}

void SettingsState::transitionToMenu()
{
	this->stateMachine.changeState(StateID::Menu);
}