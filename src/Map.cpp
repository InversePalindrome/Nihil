/*
Copyright (c) 2017 InversePalindrome
Nihil - Map.cpp
InversePalindrome.com
*/


#include "Map.hpp"

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>


Map::Map(const std::string& filePath, const sf::Vector2f& chunkSize, b2World& world) :
	chunkSize(chunkSize),
	world(world)
{
	load(filePath);
}

void Map::load(const std::string& filePath)
{
	this->layers.clear();

	map.load(filePath);

	for (std::size_t i = 0; i < this->map.getLayers().size(); ++i)
	{
		this->layers.push_back(std::make_unique<Layer>(this->map, i, this->chunkSize));
	}
	
	this->addTileCollisions();
}

void Map::addTileCollisions()
{
	const auto& layers = this->map.getLayers();

	for (const auto& layer : layers)
	{
		if (layer->getType() == tmx::Layer::Type::Object)
		{
			const auto& objects = dynamic_cast<tmx::ObjectGroup*>(layer.get())->getObjects();

			for (const auto& object : objects)
			{
				const auto& AABB = object.getAABB();

				b2BodyDef bodyDefinition;
				bodyDefinition.type = b2_staticBody;
				bodyDefinition.position.Set((AABB.left + AABB.width / 2.f)  * this->metersPerPixel, -(AABB.top - AABB.height / 2.f) * this->metersPerPixel);

				b2PolygonShape shape;
				shape.SetAsBox(object.getAABB().width * this->metersPerPixel, object.getAABB().height * this->metersPerPixel);
				
				b2FixtureDef fixture;
				fixture.shape = &shape;
				
				auto* tile = this->world.CreateBody(&bodyDefinition);
				tile->CreateFixture(&fixture);
			}
		}
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& layer : this->layers)
	{
		layer->draw(target, states);
	}
}