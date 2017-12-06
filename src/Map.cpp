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
#include <Box2D/Collision/Shapes/b2ChainShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
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
		this->layers.push_back(std::make_unique<Layer>(this->map, i, sf::Vector2f(this->bounds.width, this->bounds.height )));
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
	std::vector<std::tuple<std::int32_t, std::string, sf::Vector2f>> objectsData;
	std::vector<std::tuple<std::int32_t, std::string, sf::Vector2f>> entitiesData;

	std::int32_t entityID = 0;

	EntityProperties entityProperties;

	for (auto& object : objectLayer->getObjects())
	{
		const auto& AABB = object.getAABB();
		
		Properties properties;

		for (const auto& property : object.getProperties())
		{
			properties.emplace(property.getName(), property);
		}

		if (object.getType() == "Object" || object.getType() == "Entity")
		{
			++entityID;

			if (properties.count("File"))
			{
				if (object.getType() == "Entity")
				{
					entityProperties.emplace(-entityID, properties);

					entitiesData.push_back(std::make_tuple(-entityID, properties["File"].getStringValue(),
						sf::Vector2f(object.getPosition().x + AABB.width / 2.f, object.getPosition().y + AABB.height / 2.f)));
				}
				else if (object.getType() == "Object")
				{
					entityProperties.emplace(entityID, properties);

					objectsData.push_back(std::make_tuple(entityID, properties["File"].getStringValue(),
						sf::Vector2f(object.getPosition().x + AABB.width / 2.f, object.getPosition().y + AABB.height / 2.f)));
				}
			}
		}
		else
		{
			b2BodyDef bodyDefinition;
			bodyDefinition.type = b2_staticBody;
			bodyDefinition.position.Set(UnitConverter::pixelsToMeters(object.getPosition().x + AABB.width / 2.f), UnitConverter::pixelsToMeters(-(object.getPosition().y + AABB.height / 2.f)));

			b2FixtureDef fixtureDef;
			b2PolygonShape polygon;
			b2CircleShape circle;
			b2ChainShape chain;
			
			switch (object.getShape())
			{
			case tmx::Object::Shape::Rectangle:
			{
				polygon.SetAsBox(UnitConverter::pixelsToMeters(AABB.width / 2.f), UnitConverter::pixelsToMeters(AABB.height / 2.f));
				fixtureDef.shape = &polygon;
			}
				break;
			case tmx::Object::Shape::Ellipse:
			{
				circle.m_radius = UnitConverter::pixelsToMeters(AABB.width / 2.f);
				fixtureDef.shape = &circle;
			}
				break;
			case tmx::Object::Shape::Polygon:
			{
				std::vector<b2Vec2> points;
				
				for (const auto& point : object.getPoints())
				{
					points.push_back({ UnitConverter::pixelsToMeters(point.x), UnitConverter::pixelsToMeters(-point.y) });
				}

				polygon.Set(points.data(), points.size());
				fixtureDef.shape = &polygon;
			}
				break;
			case tmx::Object::Shape::Polyline:
			{
				std::vector<b2Vec2> points;

				for (const auto& point : object.getPoints())
				{
					points.push_back({ UnitConverter::pixelsToMeters(point.x), UnitConverter::pixelsToMeters(-point.y) });
				}

				chain.CreateChain(points.data(), points.size());
				
				fixtureDef.shape = &chain;
				break;
			}
			}

			if (object.getType() == "Sensor")
			{
				fixtureDef.isSensor = true;
			}

			auto* staticObject = this->world.CreateBody(&bodyDefinition);
			auto* fixture = staticObject->CreateFixture(&fixtureDef);
			
;			for (const auto& property : properties)
			{
				if(property.second.getName() == "PathwayData")
				{
					std::string pathwayData = property.second.getStringValue();
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

           if (properties.count("ID"))
           {
	          this->collisionsData.push_back(CollisionData(fixture, static_cast<ObjectType>(properties["ID"].getIntValue()), properties));
	          this->collisionsData.back().fixture->SetUserData(&this->collisionsData.back());
           }
		}
	}

	this->componentSerializer.setProperties(entityProperties);

	this->componentSerializer.saveBlueprint("Objects-" + game.getCurrentLevel() + ".txt", objectsData);
	this->componentSerializer.saveBlueprint("Entities-" + game.getCurrentLevel() + ".txt", entitiesData);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->background);

	for (const auto& layer : this->layers)
	{
		layer->draw(target, states);
	}
}