/*
Copyright (c) 2017 InversePalindrome
Nihil - InputHandler.cpp
InversePalindrome.com
*/


#include "InputHandler.hpp"


InputHandler::InputHandler()
{
	keyBindings[ActionID::Left] = thor::Action(sf::Keyboard::A);
	keyBindings[ActionID::Right] = thor::Action(sf::Keyboard::D);
	keyBindings[ActionID::Jump] = thor::Action(sf::Keyboard::Space);
}

void InputHandler::update(sf::Window& window)
{
	this->keyBindings.update(window);
}

void InputHandler::pushEvent(const sf::Event& event)
{
	this->keyBindings.pushEvent(event);
}

bool InputHandler::isActive(ActionID actionID) const
{
	return this->keyBindings.isActive(actionID);
}

void InputHandler::changeKey(ActionID actionID, thor::Action action)
{
	this->keyBindings[actionID] = action;
}

void InputHandler::clearEvents()
{
	this->keyBindings.clearEvents();
}
