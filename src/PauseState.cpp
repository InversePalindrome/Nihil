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
	shopButton(sfg::Button::Create("  \tShop\t ")),
	settingsButton(sfg::Button::Create("Settings")),
	quitButton(sfg::Button::Create("\t\tQuit\t\t"))
{
	resumeButton->SetPosition(sf::Vector2f(835.f, 500.f));
	resumeButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&stateMachine]() 
	{ 
		auto menu = stateMachine[stateMachine.size() - 2].get();

		menu->showWidgets(true);

		stateMachine.popState();
	});

	shopButton->SetPosition(sf::Vector2f(835.f, 650.f));
	shopButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&stateMachine]() { stateMachine.pushState(StateID::Shop); });

	settingsButton->SetPosition(sf::Vector2f(835.f, 800.f));
	settingsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&stateMachine]() { stateMachine.pushState(StateID::Settings); });

	quitButton->SetPosition(sf::Vector2f(835.f, 950.f));
	quitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() { transitionToMenu(); });

	stateData.guiManager.addWidget(resumeButton);
	stateData.guiManager.addWidget(shopButton);
	stateData.guiManager.addWidget(settingsButton);
	stateData.guiManager.addWidget(quitButton);
}

void PauseState::handleEvent(const sf::Event& event)
{
	if (this->stateData.inputHandler.isActive(Action::Escape))
	{
		auto menu = this->stateMachine[stateMachine.size() - 2].get();

		menu->showWidgets(true);

		this->stateMachine.popState();
	}
}

void PauseState::update(float deltaTime)
{
}

void PauseState::draw()
{

}

bool PauseState::isTransparent() const
{
	return true;
}

void PauseState::showWidgets(bool showStatus)
{
	this->resumeButton->Show(showStatus);
	this->settingsButton->Show(showStatus);
	this->shopButton->Show(showStatus);
	this->quitButton->Show(showStatus);
}

void PauseState::transitionToMenu()
{ 
	this->stateData.inputHandler.clearCallbacks();

	this->stateMachine.clearStates();
	this->stateMachine.pushState(StateID::Menu);
}