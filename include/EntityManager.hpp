/*
Copyright (c) 2017 InversePalindrome
Nihil - EntityManager.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "System.hpp"
#include "InputHandler.hpp"
#include "ResourceManager.hpp"

#include <Box2D/Dynamics/b2World.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <string>
#include <memory>
#include <unordered_map>


class EntityManager 
{
public:
	EntityManager(b2World& world, ResourceManager& resourceManager, InputHandler& inputHandler);

	void update(float deltaTime);
	void draw(sf::RenderTarget& target);

private:
	Entities entityManager;
	Events eventManager;

	ResourceManager& resourceManager;
	InputHandler& inputHandler;

	std::unordered_map<std::string, std::unique_ptr<System>> systems;
};