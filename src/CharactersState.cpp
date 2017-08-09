/*
Copyright (c) 2017 InversePalindrome
Nihil - CharactersState.cpp
InversePalindrome.com
*/


#include "CharactersState.hpp"
#include "StateMachine.hpp"


CharactersState::CharactersState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	background(stateData.resourceManager.getTexture(TexturesID::MenuBackground)),
	backButton(sfg::Button::Create("BACK"))
{
	background.setScale(stateData.window.getSize().x / background.getGlobalBounds().width, stateData.window.getSize().y / background.getGlobalBounds().height);

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
	this->stateData.window.draw(this->background);
}

void CharactersState::transitionToMenu()
{
	this->stateMachine.popState();
}