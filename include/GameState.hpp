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
#include "GameHud.hpp"

#include <Box2D/Dynamics/b2World.h>

#include <SFML/Graphics/View.hpp>

#include <memory>
#include <vector>
#include <functional>


class GameState : public State
{
public:
	GameState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
    b2World world;
	EntityManager entityManager;
	CollisionsData collisionsData;
	Map map;
	sf::View camera;
	std::vector<std::function<void()>> callbacks;

	CollisionHandler collisionHandler;

	std::size_t level;

	GameHud gameHud;

	void updateCamera();
	void changeLevel(std::size_t level);

	void setDisplayHitpoints(Entity entity);
};