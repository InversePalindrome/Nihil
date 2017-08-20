/*
Copyright (c) 2017 InversePalindrome
Nihil - Map.hpp
InversePalindrome.com
*/


#pragma once

#include "Layer.hpp"
#include "CollisionData.hpp"
#include "EntityManager.hpp"
#include "ResourceManager.hpp"

#include <tmxlite/Map.hpp>

#include <Box2D/Dynamics/b2World.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>
#include <unordered_map>


class Map : public sf::Drawable
{
public:
	Map(const sf::Vector2f& chunkSize, b2World& world, EntityManager& entityManager, ResourceManager& resourceManager, CollisionsData& collisionsData);

	void load(const std::string& filePath);
	
	sf::FloatRect getBounds() const;
	std::string getCurrentFilePath() const;

private:
	tmx::Map map;
	sf::Sprite background;
	sf::Vector2f chunkSize;
	sf::FloatRect bounds;
	std::string filePath;
	std::vector<std::unique_ptr<Layer>> layers;
	std::unordered_map<std::string, ObjectType> staticObjects;

	b2World& world;
	EntityManager& entityManager;
	ResourceManager& resourceManager;
	CollisionsData& collisionsData;

	void addBackgroundImage();
	void addTileCollisions();

	ObjectType findObjectType(const std::string& tileName);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
