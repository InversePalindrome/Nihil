/*
Copyright (c) 2017 InversePalindrome
Nihil - ControlSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "InputHandler.hpp"
#include "Callbacks.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


class ControlSystem : public System
{
public:
	ControlSystem(Entities& entities, Events& events, sf::RenderWindow& window);

	void addControl(Entity entity);

	void handleEvent(const sf::Event& event);

	virtual void update(float deltaTime) override;

private:
	Callbacks callbacks;
	InputHandler inputHandler;
	sf::RenderWindow& window;

	void setMidAirStatus(Entity entity, bool midAirStatus);
};