/*
Copyright (c) 2017 InversePalindrome
Nihil - SettingsState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <SFGUI/Button.hpp>


class SettingsState : public State
{
public:
	SettingsState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
	sfg::Button::Ptr backButton;

	void transitionToMenu();
};