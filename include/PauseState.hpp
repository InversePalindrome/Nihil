/*
Copyright (c) 2017 InversePalindrome
Nihil - PauseState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"


class PauseState : public State
{
public:
	PauseState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
	virtual bool isTransparent() const override;
};