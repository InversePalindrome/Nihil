/*
Copyright (c) 2017 InversePalindrome
Nihil - GameState.cpp
InversePalindrome.com
*/


#include "GameState.hpp"
#include "StateMachine.hpp"


GameState::GameState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	world(b2Vec2(0.f, -9.8f)),
	map("Resources/Files/Level1.tmx", sf::Vector2f(8172.f, 1536.f), world),
	camera(stateData.window.getDefaultView()),
	entityManager(world, stateData.resourceManager, stateData.inputHandler)
{
	world.SetContactListener(&collisionHandler);

	stateData.window.setView(this->camera);
}

void GameState::handleEvent(const sf::Event& event)
{
}

void GameState::update(float deltaTime)
{
	const float timeStep = 1.f / 60.f;
	const std::size_t velocityIterations = 6u;
	const std::size_t positionIterations = 2u;

	this->world.Step(timeStep, velocityIterations, positionIterations);

	this->entityManager.update(deltaTime);
	this->updateCamera();
}

void GameState::draw()
{
	this->stateData.window.draw(this->map);
	this->entityManager.draw(this->stateData.window);
}

void GameState::updateCamera()
{
	const auto& playerPosition = this->entityManager.getEntities().
		get_entities<Controllable, PositionComponent>().back().get_component<PositionComponent>().getPosition();
	
	if (playerPosition.x > this->camera.getSize().x / 2.f && playerPosition.x < this->map.getBounds().width - this->camera.getSize().x / 2.f)
	{
		this->camera.setCenter(playerPosition.x, this->camera.getCenter().y);
		this->stateData.window.setView(this->camera);
	}
}