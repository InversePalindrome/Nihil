/*
Copyright (c) 2017 InversePalindrome
Nihil - InputHandler.hpp
InversePalindrome.com
*/


#pragma once

#include <Thor/Input/ActionMap.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>


class InputHandler
{
public:
	InputHandler();

	void update(sf::Window& window);

	void pushEvent(const sf::Event& event);

	bool isActive(const std::string& actionID) const;

	void changeKey(const std::string& actionID, thor::Action action);

	void clearEvents();

private:
	thor::ActionMap<std::string> keyBindings;
};