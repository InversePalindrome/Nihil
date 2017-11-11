/*
Copyright (c) 2017 InversePalindrome
Nihil - Map.cpp
InversePalindrome.com
*/


#include "Map.hpp"
#include "FilePaths.hpp"
#include "UnitConverter.hpp"

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

#include <tuple>
#include <sstream>
#include <unordered_map>


Map::Map(Game& game, b2World& world, ComponentSerializer& componentSerializer, ResourceManager& resourceManager,
	CollisionsData& collisionsData, Pathways& pathways) :
	game(game),
	world(world),
	componentSerializer(componentSerializer),
	resourceManager(resourceManager),
	collisionsData(collisionsData),
	pathways(pathways)
{
}

void Map::load(const std::string& fileName)
{
	this->layers.clear();
	this->pathways.clear();
	
	this->map.load(Path::levels / fileName);
	this->fileName = Path::levels / fileName;
	this->bounds = { this->map.getBounds().left, this->map.getBounds().top, this->map.getBounds().width, this->map.getBounds().height };
	
	for (std::size_t i = 0; i < this->map.getLayers().size(); ++i)
	{
		this->layers.push_back(std::make_unique<Layer>(this->map, i, sf::Vector2f(this->bounds.width, this->bounds.height)));
	}

	this->parseMap();
}

void Map::parseMap()
{
	for (const auto& layer : this->map.getLayers())
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
	return this->fileName;
}

void Map::addImage(tmx::ImageLayer* imageLayer)
{
	auto backgroundID = static_cast<TexturesID>(imageLayer->getProperties().back().getIntValue());

	auto& backgroundTexture = this->resourceManager.getTexture(backgroundID);
	backgroundTexture.setRepeated(true);

	this->background.setTexture(backgroundTexture);
	this->background.setTextureRect({ 0, 0, static_cast<int>(this->bounds.width), static_cast<int>(this->bounds.height) });
}

void Map::addObjects(tmx::ObjectGroup* objectLayer)
{
	std::vector<std::tuple<std::int32_t, std::string, sf::Vector2f>> staticObjectFiles;
	std::vector<std::tuple<std::int32_t, std::string, sf::Vector2f>> entityFiles;

	std::int32_t entityID = 0;

	for (const auto& object : objectLayer->getObjects())
	{
		const auto& AABB = object.getAABB();

		if (object.getType() == "Entity")
		{
			++entityID;

			for (const auto& property : object.getProperties())
			{
				if (property.getName() == "ObjectFile")
				{
					staticObjectFiles.push_back(std::make_tuple(entityID, property.getStringValue(), sf::Vector2f(object.getPosition().x + AABB.width / 2.f, object.getPosition().y + AABB.height / 2.f )));
				}
				else if (property.getName() == "EntityFile")
				{
					entityFiles.push_back(std::make_tuple(-entityID, property.getStringValue(), sf::Vector2f(object.getPosition().x + AABB.width / 2.f, object.getPosition().y + AABB.height / 2.f )));
				}
			}
		}
		else
		{
			b2BodyDef bodyDefinition;
			bodyDefinition.type = b2_staticBody;
			bodyDefinition.position.Set(UnitConverter::pixelsToMeters(object.getPosition().x + AABB.width / 2.f), UnitConverter::pixelsToMeters(-(object.getPosition().y + AABB.height / 2.f)));

			b2PolygonShape shape;
			shape.SetAsBox(UnitConverter::pixelsToMeters(AABB.width / 2.f), UnitConverter::pixelsToMeters(AABB.height / 2.f));

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;

			if (object.getType() == "Sensor")
			{
				fixtureDef.isSensor = true;
			}

			auto* staticObject = this->world.CreateBody(&bodyDefinition);
			auto* fixture = staticObject->CreateFixture(&fixtureDef);

			std::unordered_map<std::string, tmx::Property> properties;
			
;			for (const auto& property : object.getProperties())
			{
	            properties.emplace(property.getName(), property);
				
				if(property.getName() == "PathwayData")
				{
					std::string pathwayData = property.getStringValue();
					std::istringstream iStream(pathwayData);

					std::size_t pathwayIndex = 0u;
					std::size_t waypointStep = 0u;

					iStream >> pathwayIndex >> waypointStep;

					if (!this->pathways.count(pathwayIndex))
					{
						this->pathways.emplace(pathwayIndex, Pathway());
					}

					this->pathways[pathwayIndex].addWaypoint(Waypoint(
					{ object.getPosition().x + AABB.width / 2.f, object.getPosition().y + AABB.height / 2.f }, waypointStep));
				}
			}

      	   this->collisionsData.push_back(CollisionData(fixture, static_cast<ObjectType>(properties["ID"].getIntValue()), properties));
           this->collisionsData.back().fixture->SetUserData(&this->collisionsData.back());
		}
	}
	
	this->componentSerializer.saveBlueprint("Objects-" + game.getCurrentLevel() + ".txt", staticObjectFiles);
	this->componentSerializer.saveBlueprint("Entities-" + game.getCurrentLevel() + ".txt", entityFiles);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->background);

	for (const auto& layer : this->layers)
	{
		layer->draw(target, states);
	}
}