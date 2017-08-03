/*
Copyright (c) 2017 InversePalindrome
Nihil - State.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/Window/Event.hpp>


struct StateData;
class StateMachine;

class State
{
public:
	State(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw() = 0;

	virtual bool isTransparent() const;

protected:
	StateMachine& stateMachine;
	StateData& stateData;
};