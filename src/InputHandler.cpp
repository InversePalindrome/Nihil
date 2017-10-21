/*
Copyright (c) 2017 InversePalindrome
Nihil - InputHandler.cpp
InversePalindrome.com
*/


#include "InputHandler.hpp"


InputHandler::InputHandler()
{
	keyBindings["Move Left"] = thor::Action(sf::Keyboard::A);
	keyBindings["Move Right"] = thor::Action(sf::Keyboard::D);
	keyBindings["Jump"] = thor::Action(sf::Keyboard::Space);
	keyBindings["Shoot"] = thor::Action(sf::Keyboard::S);
	keyBindings["Escape"] = thor::Action(sf::Keyboard::Escape, thor::Action::ReleaseOnce);
	keyBindings["Inventory"] = thor::Action(sf::Keyboard::E, thor::Action::ReleaseOnce);
}

void InputHandler::update(sf::Window& window)
{
	this->keyBindings.update(window);
}

void InputHandler::invokeCallbacks()
{
	this->keyBindings.invokeCallbacks(this->keyCallbacks, nullptr);
}

void InputHandler::invokeCallbacks(sf::Window& window)
{
	this->keyBindings.invokeCallbacks(this->keyCallbacks, &window);
}

void InputHandler::pushEvent(const sf::Event& event)
{
	this->keyBindings.pushEvent(event);
}

bool InputHandler::isActive(const std::string& actionID) const
{
	return this->keyBindings.isActive(actionID);
}

void InputHandler::addCallback(const std::string& key, const std::function<void()>& callback)
{
	this->keyCallbacks.connect0(key, callback);
}

void InputHandler::removeCallback(const std::string& key)
{
	this->keyCallbacks.clearConnections(key);
}

void InputHandler::changeKey(const std::string& actionID, thor::Action action)
{
	this->keyBindings[actionID] = action;
}

void InputHandler::clearEvents()
{
	this->keyBindings.clearEvents();
}

void InputHandler::clearCallbacks()
{
	this->keyCallbacks.clearAllConnections();
}
