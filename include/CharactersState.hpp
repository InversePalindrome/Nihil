/*
Copyright (c) 2017 InversePalindrome
Nihil - CharactersState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"


class CharactersState : public State
{
public:
	CharactersState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;
};
