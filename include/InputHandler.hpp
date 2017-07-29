/*
Copyright (c) 2017 InversePalindrome
Nihil - InputHandler.hpp
InversePalindrome.com
*/


#pragma once

#include <Thor/Input/ActionMap.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>


enum class ActionID { Left, Right};

class InputHandler
{
public:
	InputHandler();

	void update(sf::Window& window);

	void pushEvent(const sf::Event& event);

	bool isActive(ActionID actionID) const;

	void changeKey(ActionID actionID, thor::Action action);

	void clearEvents();

private:
	thor::ActionMap<ActionID> keyBindings;
};