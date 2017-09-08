/*
Copyright (c) 2017 InversePalindrome
Nihil - PauseState.cpp
InversePalindrome.com
*/


#include "PauseState.hpp"
#include "StateMachine.hpp"


PauseState::PauseState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	resumeButton(sfg::Button::Create(" Resume ")),
	settingsButton(sfg::Button::Create("Settings")),
	quitButton(sfg::Button::Create("\t\tQuit\t\t"))
{
	resumeButton->SetPosition(sf::Vector2f(850.f, 625.f));
	resumeButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&stateMachine]() { stateMachine.popState(); });

	settingsButton->SetPosition(sf::Vector2f(850.f, 775.f));
	settingsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToSettings(); });

	quitButton->SetPosition(sf::Vector2f(850.f, 925.f));
	quitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToMenu(); });

	stateData.guiManager.addWidget(resumeButton);
	stateData.guiManager.addWidget(settingsButton);
	stateData.guiManager.addWidget(quitButton);
}

void PauseState::handleEvent(const sf::Event& event)
{
	if (this->stateData.inputHandler.isActive("Escape"))
	{
		this->stateMachine.popState();
	}
}

void PauseState::update(float deltaTime)
{

}

void PauseState::draw()
{
	this->resumeButton->Show(true);
	this->settingsButton->Show(true);
	this->quitButton->Show(true);
}

bool PauseState::isTransparent() const
{
	return true;
}

void PauseState::transitionToMenu()
{ 
	this->stateMachine.clearStates();
	this->stateMachine.pushState(StateID::Menu);
}

void PauseState::transitionToSettings()
{
	this->stateMachine.pushState(StateID::Settings);
}