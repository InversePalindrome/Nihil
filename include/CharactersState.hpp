/*
Copyright (c) 2017 InversePalindrome
Nihil - CharactersState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <SFGUI/Box.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/RadioButtonGroup.hpp>

#include <SFML/Graphics/Sprite.hpp>


class CharactersState : public State
{
public:
	CharactersState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
	sf::Sprite background;

	sfg::Button::Ptr backButton;
	sfg::Box::Ptr scrolledWindowBox;
	sfg::Scale::Ptr scrollbarScale;
	sfg::Adjustment::Ptr scrollbarAdjustment;
	sfg::ScrolledWindow::Ptr scrolledWindow;
	sfg::RadioButtonGroup::Ptr characterButtons;

	void loadCharacters(const std::string& filePath);

	void selectedCharacter(const std::string& character);

	void transitionToMenu();
};
