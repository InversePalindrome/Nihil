/*
Copyright (c) 2017 InversePalindrome
Nihil - Map.cpp
InversePalindrome.com
*/


#include "Map.hpp"
#include "UnitConverter.hpp"

#include <tmxlite/ImageLayer.hpp>

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>


Map::Map(const sf::Vector2f& chunkSize, b2World& world, EntityManager& entityManager, ResourceManager& resourceManager, CollisionsData& collisionsData) :
	chunkSize(chunkSize),
	world(world),
	entityManager(entityManager),
	resourceManager(resourceManager),
	collisionsData(collisionsData)
{
}

void Map::load(const std::string& filePath)
{
	this->layers.clear();

	this->map.load(filePath);
	this->filePath = filePath;

	for (std::size_t i = 0; i < this->map.getLayers().size(); ++i)
	{
		this->layers.push_back(std::make_unique<Layer>(this->map, i, this->chunkSize));
	}

	this->bounds = sf::FloatRect(this->map.getBounds().left, this->map.getBounds().top, this->map.getBounds().width, this->map.getBounds().height);

	this->addBackgroundImage();
	this->addTileCollisions();
	this->addEntities();
}

sf::FloatRect Map::getBounds() const
{
	return this->bounds;
}

std::string Map::getCurrentFilePath() const
{
	return this->filePath;
}

void Map::addBackgroundImage()
{
	const auto& layers = this->map.getLayers();

	for (const auto& layer : layers)
	{
		if (layer->getType() == tmx::Layer::Type::Image)
		{
			TexturesID backgroundID = static_cast<TexturesID>(dynamic_cast<tmx::ImageLayer*>(layer.get())->getProperties().back().getIntValue());

			auto& backgroundTexture = this->resourceManager.getTexture(backgroundID);
			backgroundTexture.setRepeated(true);

			this->background.setTexture(backgroundTexture);
			this->background.setTextureRect(sf::IntRect(0.f, 0.f, this->bounds.width, this->bounds.height));
		}
	}
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
				if (object.getType() != "Entity")
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

					for (const auto& property : object.getProperties())
					{
						if (property.getName() == "ID")
						{
							this->collisionsData.push_back(CollisionData(tile, static_cast<ObjectType>(property.getIntValue()), object.getProperties()));
							this->collisionsData.back().body->SetUserData(&this->collisionsData.back());
						}
					}
				}
			}
			
		}
	}
}

void Map::addEntities()
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

				for (const auto& property : object.getProperties())
				{
					if (property.getName() == "File")
					{
						this->entityManager.createEntity(property.getStringValue(), sf::Vector2f(AABB.left + AABB.width / 2.f, AABB.top + AABB.height / 2.f));
					}
				}
			}

		}
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->background);

	for (const auto& layer : this->layers)
	{
		layer->draw(target, states);
	}
}