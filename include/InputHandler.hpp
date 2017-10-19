/*
Copyright (c) 2017 InversePalindrome
Nihil - InputHandler.hpp
InversePalindrome.com
*/


#pragma once

#include <Thor/Input/ActionMap.hpp>
#include <Thor/Input/EventSystem.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>


class InputHandler
{
public:
	InputHandler();

	void update(sf::Window& window);
	void invokeCallbacks();
	void invokeCallbacks(sf::Window& window);

	void pushEvent(const sf::Event& event);

	bool isActive(const std::string& actionID) const;

	void addCallback(const std::string& key, const std::function<void()>& callback);
	void removeCallback(const std::string& key);

	void changeKey(const std::string& actionID, thor::Action action);

	void clearEvents();
	void clearCallbacks();

private:
	thor::ActionMap<std::string> keyBindings;
	thor::ActionMap<std::string>::CallbackSystem keyCallbacks;
};