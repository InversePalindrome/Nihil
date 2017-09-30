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
#include "AISystem.hpp"
#include "CombatSystem.hpp"
#include "AnimatorSystem.hpp"
#include "SoundSystem.hpp"
#include "EffectsSystem.hpp"
#include "AutomatorSystem.hpp"
#include "ItemsSystem.hpp"
#include "UnitConverter.hpp"

#include <fstream>


EntityManager::EntityManager(b2World& world, ResourceManager& resourceManager, SoundManager& soundManager, InputHandler& inputHandler, CollisionsData& collisionsData, Pathways& pathways) :
	world(world),
	componentParser(entityManager, resourceManager, world),
	componentSerializer(entityManager)
{
	entityManager.set_event_manager(eventManager);
	
	systems["Render"] = std::make_unique<RenderSystem>(entityManager, eventManager);
	systems["Control"] = std::make_unique<ControlSystem>(entityManager, eventManager, inputHandler);
	systems["State"] = std::make_unique<StateSystem>(entityManager, eventManager);
	systems["Physics"] = std::make_unique<PhysicsSystem>(entityManager, eventManager, world, collisionsData);
	systems["AI"] = std::make_unique<AISystem>(entityManager, eventManager, pathways);
	systems["Combat"] = std::make_unique<CombatSystem>(entityManager, eventManager, componentParser);
	systems["Animator"] = std::make_unique<AnimatorSystem>(entityManager, eventManager);
	systems["Sound"] = std::make_unique<SoundSystem>(entityManager, eventManager, soundManager);
	systems["Effects"] = std::make_unique<EffectsSystem>(entityManager, eventManager);
	systems["Automator"] = std::make_unique<AutomatorSystem>(entityManager, eventManager);
	systems["Item"] = std::make_unique<ItemsSystem>(entityManager, eventManager);
}

Entities& EntityManager::getEntities()
{
	return this->entityManager;
}

Events& EntityManager::getEvents()
{
	return this->eventManager;
}

ComponentSerializer& EntityManager::getComponentSerializer()
{
	return this->componentSerializer;
}

void EntityManager::update(float deltaTime)
{
	for (auto& system : this->systems)
	{
		system.second->update(deltaTime);
	}
}

Entity EntityManager::createEntity(const std::string& pathFile)
{
	return this->componentParser.parseComponents(pathFile);
}

Entity EntityManager::createEntity(const std::string& pathFile, const sf::Vector2f& position)
{
	auto& entity = this->createEntity(pathFile);
	
	if (entity.has_component<PositionComponent>())
	{
		entity.get_component<PositionComponent>().setPosition(position);
	}
	if (entity.has_component<PhysicsComponent>())
	{
		entity.get_component<PhysicsComponent>().setPosition(
			b2Vec2(UnitConverter::pixelsToMeters(position.x), UnitConverter::pixelsToMeters(-position.y)));
	}

	return entity;
}

void EntityManager::parseEntities(const std::string& pathFile)
{
	this->componentParser.parseEntities(pathFile);
}

void EntityManager::parseBlueprint(const std::string& pathFile)
{
	this->componentParser.parseBlueprint(pathFile);
}

void EntityManager::destroyEntity(Entity entity)
{
	if (entity.get_status() != entityplus::entity_status::DELETED)
	{
		if (entity.has_component<PhysicsComponent>() && entity.get_component<PhysicsComponent>().getBody())
		{
			this->world.DestroyBody(entity.get_component<PhysicsComponent>().getBody());
		}

		entity.destroy();
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

void EntityManager::saveEntities(const std::string& pathFile)
{
	this->componentSerializer.serialize(pathFile);
}

void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*dynamic_cast<RenderSystem*>(this->systems.at("Render").get()));
}