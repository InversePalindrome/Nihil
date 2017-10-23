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

#include <unordered_map>


enum class Action { MoveLeft, MoveRight, Jump, Shoot, Escape, Inventory, Size };

class InputHandler
{
public:
	InputHandler();

	void update(sf::Window& window);
	void invokeCallbacks();
	void invokeCallbacks(sf::Window& window);

	void pushEvent(const sf::Event& event);

	bool isActive(Action action) const;

	void addCallback(Action action, const std::function<void()>& callback);
	void removeCallback(Action action);

	void changeKey(Action action, sf::Keyboard::Key key);

	void clearEvents();
	void clearCallbacks();

	void saveData();

private:
	thor::ActionMap<Action> keyBindings;
	thor::ActionMap<Action>::CallbackSystem keyCallbacks;

	std::unordered_map<Action, std::pair<std::size_t, std::size_t>> keyCodes;
};