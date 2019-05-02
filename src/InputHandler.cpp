/*
Copyright (c) 2017 InversePalindrome
Nihil - InputHandler.cpp
InversePalindrome.com
*/


#include "InputHandler.hpp"
#include "FilePaths.hpp"

#include <fstream>


InputHandler::InputHandler()
{
    std::ifstream inFile(Path::miscellaneous / "InputKeys.txt");

    std::size_t actionID = 0u, keyID = 0u, actionType = 0u;

    while (inFile >> actionID >> keyID >> actionType)
    {
        keyBindings[static_cast<Action>(actionID)] =
            thor::Action(static_cast<sf::Keyboard::Key>(keyID), static_cast<thor::Action::ActionType>(actionType));

        keyCodes[Action{ actionID }] = { keyID, actionType };
    }
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

bool InputHandler::isActive() const
{
    for (std::size_t i = 0u; i < static_cast<std::size_t>(Action::Size); ++i)
    {
        if (this->isActive(Action{ i }))
        {
            return true;
        }
    }

    return false;
}

bool InputHandler::isActive(Action action) const
{
    return this->keyBindings.isActive(action);
}

void InputHandler::addCallback(Action action, const std::function<void()>& callback)
{
    this->keyCallbacks.connect0(action, callback);
}

void InputHandler::removeCallback(Action action)
{
    this->keyCallbacks.clearConnections(action);
}

void InputHandler::changeKey(Action action, sf::Keyboard::Key key)
{
    this->keyBindings[action] = thor::Action(key);
    this->keyCodes[action] = { static_cast<std::size_t>(key), 0u };
}

void InputHandler::clearEvents()
{
    this->keyBindings.clearEvents();
}

void InputHandler::clearCallbacks()
{
    this->keyCallbacks.clearAllConnections();
}

void InputHandler::saveData()
{
    std::ofstream outFile(Path::miscellaneous / "InputKeys.txt");

    for (const auto& [actionID, keyInfo] : this->keyCodes)
    {
        outFile << static_cast<std::size_t>(actionID) << ' '
            << keyInfo.first << ' ' << keyInfo.second << '\n';
    }
}
