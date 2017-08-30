/*
Copyright (c) 2017 InversePalindrome
Nihil - GameState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"
#include "Map.hpp"
#include "CollisionData.hpp"
#include "CollisionHandler.hpp"
#include "EntityManager.hpp"
#include "HealthBar.hpp"
#include "CoinDisplay.hpp"

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

	HealthBar healthBar;
	CoinDisplay coinDisplay;

	void updateCamera();
	void changeLevel(const std::string& level);

	void setDisplayHitpoints(Entity entity);
};