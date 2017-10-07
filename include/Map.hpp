/*
Copyright (c) 2017 InversePalindrome
Nihil - Map.hpp
InversePalindrome.com
*/


#pragma once

#include "Layer.hpp"
#include "Game.hpp"
#include "Pathway.hpp"
#include "CollisionData.hpp"
#include "ResourceManager.hpp"
#include "ComponentSerializer.hpp"

#include <tmxlite/Map.hpp>
#include <tmxlite/ImageLayer.hpp>

#include <Box2D/Dynamics/b2World.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>


class Map : public sf::Drawable
{
public:
	Map(Game& game, b2World& world, ComponentSerializer& componentSerializer,
		ResourceManager& resourceManager, CollisionsData& collisionsData, Pathways& pathways);

	void load(const std::string& fileName);
	
	sf::FloatRect getBounds() const;
	std::string getCurrentFilePath() const;

private:
	tmx::Map map;
	sf::Sprite background;
	sf::FloatRect bounds;
	std::string fileName;
	std::vector<std::unique_ptr<Layer>> layers;

	Game& game;
	b2World& world;
	ComponentSerializer& componentSerializer;
	ResourceManager& resourceManager;
	CollisionsData& collisionsData;
	Pathways& pathways;

	void parseMap();

	void addImage(tmx::ImageLayer* imageLayer);
	void addObjects(tmx::ObjectGroup* objectLayer);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};