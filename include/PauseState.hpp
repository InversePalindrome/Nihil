/*
Copyright (c) 2017 InversePalindrome
Nihil - PauseState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <SFGUI/Button.hpp>


class PauseState : public State
{
public:
	PauseState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

	virtual bool isTransparent() const override;

private:
	sfg::Button::Ptr resumeButton;
	sfg::Button::Ptr settingsButton;
	sfg::Button::Ptr quitButton;
	
	void saveGame(const std::string& pathFile);

	void transitionToMenu();
	void transitionToSettings();
};