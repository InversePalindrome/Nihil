/*
Copyright (c) 2017 InversePalindrome
Nihil - GameOverState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"


class GameOverState : public State
{
public:
	GameOverState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
	virtual bool isTransparent() const override;
};