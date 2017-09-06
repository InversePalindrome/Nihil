/*
Copyright (c) 2017 InversePalindrome
Nihil - HubState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <SFGUI/Entry.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/RadioButtonGroup.hpp>

#include <SFML/Graphics/Sprite.hpp>


class HubState : public State
{
public:
	HubState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
	sf::Sprite background;

	sfg::Window::Ptr addGamePopup;
	sfg::Entry::Ptr nameEntry;

	sfg::Button::Ptr backButton;
	sfg::Button::Ptr playButton;
	sfg::Button::Ptr addButton;
	sfg::Button::Ptr deleteButton;
	sfg::RadioButtonGroup::Ptr selectionButtons;

	void loadGames(const std::string& pathFile);

	void showAddGamePopup();
	void addGame();
	void deleteGame();

	void transitionToMenu();
	void transitionToPlay();
};