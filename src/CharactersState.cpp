/*
Copyright (c) 2017 InversePalindrome
Nihil - CharactersState.cpp
InversePalindrome.com
*/


#include "CharactersState.hpp"
#include "StateMachine.hpp"

#include <SFGUI/Image.hpp>


CharactersState::CharactersState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	backButton(sfg::Button::Create())
{
	backButton->SetImage(sfg::Image::Create(stateData.resourceManager.getImage(ImagesID::BackButton)));
	backButton->SetPosition(sf::Vector2f(12.f, 65.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	stateData.guiManager.addWidget(backButton);
}

void CharactersState::handleEvent(const sf::Event& event)
{

}

void CharactersState::update(float deltaTime)
{

}

void CharactersState::draw()
{

}

void CharactersState::transitionToMenu()
{
	this->stateMachine.changeState(StateID::Menu);
}