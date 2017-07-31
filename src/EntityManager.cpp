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
#include "AnimatorSystem.hpp"


EntityManager::EntityManager(b2World& world, ResourceManager& resourceManager, InputHandler& inputHandler) :
	resourceManager(resourceManager),
	inputHandler(inputHandler)
{
	entityManager.set_event_manager(eventManager);

	systems["Render"] = std::make_unique<RenderSystem>(entityManager, eventManager);
	systems["Control"] = std::make_unique<ControlSystem>(entityManager, eventManager, inputHandler);
	systems["State"] = std::make_unique<StateSystem>(entityManager, eventManager);
	systems["Physics"] = std::make_unique<PhysicsSystem>(entityManager, eventManager, world);
	systems["Animator"] = std::make_unique<AnimatorSystem>(entityManager, eventManager);
	createEntity("B");
	
	entityManager.create_entity<Controllable>(PhysicsComponent(world, b2Vec2(1.25, 2.3f), b2Vec2(0.f, 0.f), 10.f, 1.f), PositionComponent{ {} }, SpriteComponent{ resourceManager.getTexture(TexturesID::Character), sf::IntRect(5, 100, 20, 32), sf::Vector2f(3.f, 3.f) }, StateComponent{}, AnimationComponent{ "Resources/Files/CharacterAnimations.txt" });
}

Entities& EntityManager::getEntities()
{
	return this->entityManager;
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
	auto& entity = this->entityManager.create_entity<>();
	
	entity.add_component(this->makeComponent<SpriteComponent>(std::forward_as_tuple(resourceManager.getTexture(TexturesID::Character), sf::IntRect(5, 100, 20, 32), sf::Vector2f(3.f, 3.f))));
	entity.add_component(this->makeComponent<PositionComponent>(std::forward_as_tuple(sf::Vector2f(200.f, 200.f))));
}
