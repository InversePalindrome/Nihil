/*
Copyright (c) 2017 InversePalindrome
Nihil - PauseState.cpp
InversePalindrome.com
*/


#include "PauseState.hpp"
#include "StateMachine.hpp"

#include <SFGUI/Image.hpp>


PauseState::PauseState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	resumeButton(sfg::Button::Create()),
	restartButton(sfg::Button::Create()),
	settingsButton(sfg::Button::Create()),
	quitButton(sfg::Button::Create())
{
	resumeButton->SetPosition(sf::Vector2f(900.f, 600.f));
	resumeButton->SetImage(sfg::Image::Create(stateData.resourceManager.getImage(ImagesID::ResumeButton)));
	resumeButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&stateMachine]() { stateMachine.popState(); });

	restartButton->SetPosition(sf::Vector2f(900.f, 680.f));
	restartButton->SetImage(sfg::Image::Create(stateData.resourceManager.getImage(ImagesID::RestartButton)));
	restartButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&stateMachine]() { stateMachine.changeState(StateID::Game); });

	settingsButton->SetPosition(sf::Vector2f(900.f, 760.f));
	settingsButton->SetImage(sfg::Image::Create(stateData.resourceManager.getImage(ImagesID::SettingsSmall)));
	settingsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToSettings(); });

	quitButton->SetPosition(sf::Vector2f(900.f, 840.f));
	quitButton->SetImage(sfg::Image::Create(stateData.resourceManager.getImage(ImagesID::QuitButton)));
	quitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToMenu(); });

	stateData.guiManager.addWidget(resumeButton);
	stateData.guiManager.addWidget(restartButton);
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
	this->restartButton->Show(true);
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