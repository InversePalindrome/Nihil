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


Map::Map(const sf::Vector2f& chunkSize, Game& game, b2World& world, ComponentSerializer& componentSerializer, ResourceManager& resourceManager, CollisionsData& collisionsData) :
	chunkSize(chunkSize),
	game(game),
	world(world),
	componentSerializer(componentSerializer),
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

	this->parseMap();
}

void Map::parseMap()
{
	const auto& layers = this->map.getLayers();

	for (const auto& layer : layers)
	{
		switch (layer->getType())
		{
		case tmx::Layer::Type::Image:
			this->addImage(dynamic_cast<tmx::ImageLayer*>(layer.get()));
			break;
		case tmx::Layer::Type::Object:
			this->addObjects(dynamic_cast<tmx::ObjectGroup*>(layer.get()));
			break;
		}
	}
}

sf::FloatRect Map::getBounds() const
{
	return this->bounds;
}

std::string Map::getCurrentFilePath() const
{
	return this->filePath;
}

void Map::addImage(tmx::ImageLayer* imageLayer)
{
	auto backgroundID = static_cast<TexturesID>(imageLayer->getProperties().back().getIntValue());

	auto& backgroundTexture = this->resourceManager.getTexture(backgroundID);
	backgroundTexture.setRepeated(true);

	this->background.setTexture(backgroundTexture);
	this->background.setTextureRect(sf::IntRect(0.f, 0.f, this->bounds.width, this->bounds.height));
}

void Map::addObjects(tmx::ObjectGroup* objectLayer)
{
	const auto& objects = objectLayer->getObjects();
	std::vector<std::pair<std::string, sf::Vector2f>> objectFiles;
	std::vector<std::pair<std::string, sf::Vector2f>> enemyFiles;

	for (const auto& object : objects)
	{
		const auto& AABB = object.getAABB();

		if (object.getType() == "Entity")
		{
			for (const auto& property : object.getProperties())
			{
				if (property.getName() == "ObjectFile")
				{
					objectFiles.push_back(std::make_pair(property.getStringValue(), sf::Vector2f(AABB.left + AABB.width / 2.f, AABB.top + AABB.height / 2.f)));
				}
				else if (property.getName() == "EnemyFile")
				{
					enemyFiles.push_back(std::make_pair(property.getStringValue(), sf::Vector2f(AABB.left + AABB.width / 2.f, AABB.top + AABB.height / 2.f)));
				}
			}
		}
		else
		{
			b2BodyDef bodyDefinition;
			bodyDefinition.type = b2_staticBody;
			bodyDefinition.position.Set(UnitConverter::pixelsToMeters(AABB.left + AABB.width / 2.f), UnitConverter::pixelsToMeters(-(AABB.top + AABB.height / 2.f)));

			b2PolygonShape shape;
			shape.SetAsBox(UnitConverter::pixelsToMeters(AABB.width / 2.f), UnitConverter::pixelsToMeters(AABB.height / 2.f));

			b2FixtureDef fixture;
			fixture.shape = &shape;

			auto* tile = this->world.CreateBody(&bodyDefinition);
			tile->CreateFixture(&fixture);

			std::unordered_map<std::string, tmx::Property> properties;

;			for (const auto& property : object.getProperties())
			{
	            properties.emplace(property.getName(), property);
			}
            
            this->collisionsData.push_back(CollisionData(tile, static_cast<ObjectType>(properties["ID"].getIntValue()), properties));
            this->collisionsData.back().body->SetUserData(&this->collisionsData.back());
		}
	}
	
	this->componentSerializer.saveBlueprint("Resources/Files/BlueprintObjects-" + game.getCurrentLevel() + ".txt", objectFiles);
	this->componentSerializer.saveBlueprint("Resources/Files/BlueprintEnemies-" + game.getCurrentLevel() + ".txt", enemyFiles);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->background);

	for (const auto& layer : this->layers)
	{
		layer->draw(target, states);
	}
}