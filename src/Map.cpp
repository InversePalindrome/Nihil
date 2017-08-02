/*
Copyright (c) 2017 InversePalindrome
Nihil - Map.cpp
InversePalindrome.com
*/


#include "Map.hpp"
#include "UnitConverter.hpp"

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>


Map::Map(const std::string& filePath, const sf::Vector2f& chunkSize, b2World& world, CollisionsData& collisionsData) :
	chunkSize(chunkSize),
	world(world),
	collisionsData(collisionsData)
{
	load(filePath);

	bounds = sf::FloatRect(map.getBounds().left, map.getBounds().top, map.getBounds().width, map.getBounds().height);
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

sf::FloatRect Map::getBounds() const
{
	return this->bounds;
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
				bodyDefinition.position.Set(UnitConverter::pixelsToMeters(AABB.left + AABB.width / 2.f), UnitConverter::pixelsToMeters(-(AABB.top + AABB.height / 2.f)));

				b2PolygonShape shape;
				shape.SetAsBox(UnitConverter::pixelsToMeters(AABB.width / 2.f), UnitConverter::pixelsToMeters(AABB.height / 2.f));

				b2FixtureDef fixture;
				fixture.shape = &shape;

				auto* tile = this->world.CreateBody(&bodyDefinition);
				tile->CreateFixture(&fixture);
				
				this->collisionsData.push_back(CollisionData(*tile, this->findObjectType(object.getName())));

				this->collisionsData.back().body.SetUserData(&this->collisionsData.back());
			}
		}
	}
}

ObjectType Map::findObjectType(const std::string& tileName)
{
	if (tileName == "Trap")
	{
		return ObjectType::Trap;
	}
	else
	{
		return ObjectType::Tile;
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& layer : this->layers)
	{
		layer->draw(target, states);
	}
}