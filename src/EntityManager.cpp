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
#include "CombatSystem.hpp"
#include "AnimatorSystem.hpp"
#include "SoundSystem.hpp"

#include <fstream>


EntityManager::EntityManager(b2World& world, ResourceManager& resourceManager, SoundManager& soundManager, InputHandler& inputHandler, CollisionsData& collisionsData) :
	componentParser(entityManager, resourceManager, world),
	world(world)
{
	entityManager.set_event_manager(eventManager);
	
	systems["Render"] = std::make_unique<RenderSystem>(entityManager, eventManager);
	systems["Control"] = std::make_unique<ControlSystem>(entityManager, eventManager, inputHandler);
	systems["State"] = std::make_unique<StateSystem>(entityManager, eventManager);
	systems["Physics"] = std::make_unique<PhysicsSystem>(entityManager, eventManager, world, collisionsData);
	systems["AI"] = std::make_unique<NeuralSystem>(entityManager, eventManager);
	systems["Combat"] = std::make_unique<CombatSystem>(entityManager, eventManager);
	systems["Animator"] = std::make_unique<AnimatorSystem>(entityManager, eventManager);
	systems["Sound"] = std::make_unique<SoundSystem>(entityManager, eventManager, soundManager);
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

void EntityManager::createEntities(const std::string& filePath)
{
	std::ifstream inFile(filePath);
	std::string line;

	while (std::getline(inFile, line))
	{
		this->componentParser.parseComponents(line);
	}
}

void EntityManager::destroyEntities()
{
	auto& entities = this->entityManager.get_entities();

	for (auto* body = this->world.GetBodyList(); body; )
	{
		if (body)
		{
			auto* nextBody = body->GetNext();

			this->world.DestroyBody(body);

			body = nextBody;
		}
	}

	for (auto& entity : entities)
	{
		entity.destroy();
	}
}
