/*
Copyright (c) 2017 InversePalindrome
Nihil - MenuState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <SFGUI/Button.hpp>


class MenuState : public State
{
public:
	MenuState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;
	
private:
	sfg::Button::Ptr playButton;
	sfg::Button::Ptr settingsButton;
	sfg::Button::Ptr charactersButton;

	void transitionToPlay();
	void transitionToSettings();
	void transitionToCharacters();
};