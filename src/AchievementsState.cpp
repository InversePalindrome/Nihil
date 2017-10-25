/*
Copyright (c) 2017 InversePalindrome
Nihil - AchievementsState.cpp
InversePalindrome.com
*/


#include "AchievementsState.hpp"
#include "StateMachine.hpp"


AchievementsState::AchievementsState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	backButton(sfg::Button::Create("Back"))
{
	backButton->SetPosition(sf::Vector2f(12.f, 25.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	stateData.guiManager.addWidget(backButton);
}

void AchievementsState::handleEvent(const sf::Event& event)
{

}

void AchievementsState::update(float deltaTime)
{

}

void AchievementsState::draw()
{

}

bool AchievementsState::isTransparent() const
{
	return true;
}

bool AchievementsState::isDependent() const
{
	return true;
}

void AchievementsState::transitionToMenu()
{
	this->stateMachine.popState();

	auto menu = this->stateMachine[this->stateMachine.size() - 2].get();

	menu->showWidgets(true);
}