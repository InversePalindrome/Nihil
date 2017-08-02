/*
Copyright (c) 2017 InversePalindrome
Nihil - EntityManager.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "System.hpp"
#include "InputHandler.hpp"
#include "CollisionData.hpp"
#include "ResourceManager.hpp"
#include "ComponentParser.hpp"

#include <Box2D/Dynamics/b2World.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <tuple>
#include <string>
#include <memory>
#include <unordered_map>


class EntityManager 
{
public:
	EntityManager(b2World& world, ResourceManager& resourceManager, InputHandler& inputHandler, CollisionsData& collisionData);

	Entities& getEntities();
	Events& getEvents();

	void update(float deltaTime);
	void draw(sf::RenderTarget& target);

	void createEntity(const std::string& filePath);

	template<typename Component, typename Tuple>
	Component makeComponent(Tuple& tuple);

private:
	Entities entityManager;
	Events eventManager;

	ComponentParser componentParser;

	std::unordered_map<std::string, std::unique_ptr<System>> systems;
};

template<typename Component, typename Tuple>
Component EntityManager::makeComponent(Tuple& tuple)
{
	return std::make_from_tuple<Component>(tuple);
}