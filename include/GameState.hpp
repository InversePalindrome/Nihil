/*
Copyright (c) 2017 InversePalindrome
Nihil - State.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"
#include "Map.hpp"
#include "CollisionHandler.hpp"
#include "EntityManager.hpp"

#include <Box2D/Dynamics/b2World.h>


class GameState : public State
{
public:
	GameState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
    b2World world;
	CollisionHandler collisionHandler;
	Map map;
	
	EntityManager entityManager;
};