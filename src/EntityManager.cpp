/*
Copyright (c) 2017 InversePalindrome
Nihil - EntityManager.cpp
InversePalindrome.com
*/


#include "EntityManager.hpp"
#include "RenderSystem.hpp"
#include "StateSystem.hpp"
#include "ControlSystem.hpp"
#include "PhysicsSystem.hpp"
#include "NeuralSystem.hpp"
#include "AnimatorSystem.hpp"


EntityManager::EntityManager(b2World& world, ResourceManager& resourceManager, InputHandler& inputHandler, CollisionsData& collisionsData) :
	componentParser(entityManager, resourceManager, world)
{
	entityManager.set_event_manager(eventManager);
	
	systems["Render"] = std::make_unique<RenderSystem>(entityManager, eventManager);
	systems["Control"] = std::make_unique<ControlSystem>(entityManager, eventManager, inputHandler);
	systems["State"] = std::make_unique<StateSystem>(entityManager, eventManager);
	systems["Physics"] = std::make_unique<PhysicsSystem>(entityManager, eventManager, world, collisionsData);
	systems["AI"] = std::make_unique<NeuralSystem>(entityManager, eventManager);
	systems["Animator"] = std::make_unique<AnimatorSystem>(entityManager, eventManager);
	
	createEntity("Resources/Files/Player.txt");
	createEntity("Resources/Files/Mushroom.txt");
}

Entities& EntityManager::getEntities()
{
	return this->entityManager;
}

Events& EntityManager::getEvents()
{
	return this->eventManager;
}

void EntityManager::update(float deltaTime)
{
	for (auto& system : this->systems)
	{
		system.second->update(deltaTime);
	}
}

void EntityManager::draw(sf::RenderTarget& target)
{
	dynamic_cast<RenderSystem*>(this->systems.at("Render").get())->draw(target);
}

void EntityManager::createEntity(const std::string& filePath)
{
	this->componentParser.parseComponents(filePath);
}
