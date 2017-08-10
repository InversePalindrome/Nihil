/*
Copyright (c) 2017 InversePalindrome
Nihil - GameState.hpp
InversePalindrome.com
*/


#pragma once

#include "Map.hpp"
#include "State.hpp"
#include "CollisionData.hpp"
#include "CollisionHandler.hpp"
#include "EntityManager.hpp"

#include <Box2D/Dynamics/b2World.h>

#include <SFML/Graphics/View.hpp>

#include <memory>


class GameState : public State
{
public:
	GameState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
    b2World world;
	CollisionsData collisionsData;
	Map map;
	sf::View camera;

	EntityManager entityManager;
	CollisionHandler collisionHandler;

	std::size_t level;
	bool isReadyToloadLevel;

	void updateCamera();
	void advanceToNextLevel();
};