/*
Copyright (c) 2017 InversePalindrome
Nihil - AchievementsState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <SFGUI/Button.hpp>
#include <SFGUI/ScrolledWindow.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>


class AchievementsState : public State
{
public:
	AchievementsState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

	virtual bool isTransparent() const override;
	virtual bool isDependent() const override;

private:
	sfg::Button::Ptr backButton;
	sfg::ScrolledWindow::Ptr scrolledWindow;

	sf::Sprite background;
	sf::Sprite titleBar;
	sf::Text title;

	void loadAchievements(const std::string& fileName);

	void transitionToMenu();
};