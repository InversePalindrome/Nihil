/*
Copyright (c) 2017 InversePalindrome
Nihil - EntityManager.cpp
InversePalindrome.com
*/


#include "EntityManager.hpp"
#include "RenderSystem.hpp"


EntityManager::EntityManager(ResourceManager& resourceManager) :
	resourceManager(resourceManager)
{
	entityManager.set_event_manager(eventManager);

	systems["Render"] = std::make_unique<RenderSystem>(entityManager, eventManager);

	entityManager.create_entity(PositionComponent{ sf::Vector2f(100.f, 100.f) }, SpriteComponent{ resourceManager.getTexture(TexturesID::SplashScreen) });
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