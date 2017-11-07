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


EntityManager::EntityManager(b2World& world, ResourceManager& resourceManager, SoundManager& soundManager,
	InputHandler& inputHandler, CollisionsData& collisionsData, Pathways& pathways) :
	world(world),
	componentParser(entityManager, resourceManager, world),
	componentSerializer(entityManager)
{
	entityManager.set_event_manager(eventManager);
	
	systems[typeid(RenderSystem).name()] = std::make_unique<RenderSystem>(entityManager, eventManager);
	systems[typeid(ControlSystem).name()] = std::make_unique<ControlSystem>(entityManager, eventManager, inputHandler);
	systems[typeid(StateSystem).name()] = std::make_unique<StateSystem>(entityManager, eventManager);
	systems[typeid(PhysicsSystem).name()] = std::make_unique<PhysicsSystem>(entityManager, eventManager, world, collisionsData);
	systems[typeid(AISystem).name()] = std::make_unique<AISystem>(entityManager, eventManager, pathways);
	systems[typeid(CombatSystem).name()] = std::make_unique<CombatSystem>(entityManager, eventManager, componentParser);
	systems[typeid(AnimatorSystem).name()] = std::make_unique<AnimatorSystem>(entityManager, eventManager);
	systems[typeid(SoundSystem).name()] = std::make_unique<SoundSystem>(entityManager, eventManager, soundManager);
	systems[typeid(EffectsSystem).name()] = std::make_unique<EffectsSystem>(entityManager, eventManager);
	systems[typeid(AutomatorSystem).name()] = std::make_unique<AutomatorSystem>(entityManager, eventManager);
	systems[typeid(ItemsSystem).name()] = std::make_unique<ItemsSystem>(entityManager, eventManager);
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

Entity EntityManager::createEntity(const std::string& fileName)
{
	return this->componentParser.parseComponents(fileName);
}

Entity EntityManager::createEntity(const std::string& fileName, const sf::Vector2f& position)
{
	auto entity = this->createEntity(fileName);
	
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

void EntityManager::parseEntities(const std::string& fileName)
{
	this->componentParser.parseEntities(fileName);
}

void EntityManager::parseBlueprint(const std::string& fileName)
{
	this->componentParser.parseBlueprint(fileName);
}

void EntityManager::copyBlueprint(const std::string& fileName, const std::string& copiedFileName)
{
	this->componentParser.copyBlueprint(fileName, copiedFileName);
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
	for (auto* body = this->world.GetBodyList(); body; )
	{
		if (body)
		{
			auto* nextBody = body->GetNext();

			this->world.DestroyBody(body);

			body = nextBody;
		}
	}

	for (auto& entity : this->entityManager.get_entities())
	{
		entity.destroy();
	}
}

void EntityManager::saveEntities(const std::string& fileName)
{
	this->componentSerializer.serialize(fileName);
}

void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*dynamic_cast<RenderSystem*>(this->systems.at(typeid(RenderSystem).name()).get()));
}