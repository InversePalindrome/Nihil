/*
Copyright (c) 2017 InversePalindrome
Nihil - State.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"
#include "Map.hpp"
#include "EntityManager.hpp"


class GameState : public State
{
public:
	GameState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
	EntityManager entityManager;
	Map map;
};