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
	map("Resources/Files/Level1.tmx", sf::Vector2f(2048.f, 1536.f), world),
	entityManager(world, stateData.resourceManager, stateData.inputHandler)
{
	world.SetContactListener(&collisionHandler);

	stateData.window.setView(stateData.window.getDefaultView());
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
}

void GameState::draw()
{
	this->stateData.window.draw(this->map);
	this->entityManager.draw(this->stateData.window);
}