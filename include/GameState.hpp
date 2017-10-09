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
#include "Callbacks.hpp"
#include "EntityManager.hpp"
#include "HealthBar.hpp"
#include "CoinDisplay.hpp"
#include "ItemsDisplay.hpp"
#include "Pathway.hpp"

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
	EntityManager entityManager;
	CollisionsData collisionsData;

	Map map;
	sf::View camera;
	Callbacks callbacks;
    Pathways pathways;

	CollisionHandler collisionHandler;

	HealthBar healthBar;
	CoinDisplay coinDisplay;
	ItemsDisplay itemsDisplay;

	void moveCamera(const sf::Vector2f& position);

	void updateCamera();
	void updateHealthBar(const HealthComponent& health);
	void updateCoinDisplay(const InventoryComponent& inventory);
	void updateItemsDisplay(Entity item);

	void changeLevel(const std::string& level);
	void changeEntityPosition(Entity entity, const sf::Vector2f& position);

	void saveData(const std::string& fileName);
};