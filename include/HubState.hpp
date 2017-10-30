/*
Copyright (c) 2017 InversePalindrome
Nihil - HubState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/RadioButtonGroup.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>


class HubState : public State
{
public:
	HubState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

	virtual bool isTransparent() const override;
	virtual bool isDependent() const override;

private:
	sf::Sprite titleBar;
	sf::Sprite addGameBackground;
	sf::Sprite addGameTitleBar;
	sf::Text title;

	sfg::ScrolledWindow::Ptr scrolledWindow;
	sfg::Entry::Ptr nameEntry;

	sfg::Button::Ptr backButton;
	sfg::Button::Ptr playButton;
	sfg::Button::Ptr addButton;
	sfg::Button::Ptr deleteButton;
	sfg::Box::Ptr selectionBox;
	sfg::Box::Ptr gamePopupBox;
	sfg::RadioButtonGroup::Ptr selectionButtons;

	bool isAddingGame;

	void addGames();

	void showAddGamePopup();
	void addGame();
	void deleteGame();

	void saveGames(const std::string& fileName);

	void transitionToMenu();
	void transitionToPlay();
};