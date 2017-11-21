/*
Copyright (c) 2017 InversePalindrome
Nihil - PauseState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <SFGUI/Button.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>


class PauseState : public State
{
public:
	PauseState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

	virtual bool isTransparent() const override;
	virtual void showWidgets(bool showStatus) override;

private:
	sf::Text title;
	sf::Sprite titleBar;
	sf::Sprite background;

	sfg::Button::Ptr resumeButton;
	sfg::Button::Ptr shopButton;
	sfg::Button::Ptr achievementsButton;
	sfg::Button::Ptr settingsButton;
	sfg::Button::Ptr quitButton;

	bool isBackgroundVisible;

	void resumeGame();

	void transitionToMenu();
};