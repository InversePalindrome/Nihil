/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentParser.cpp
InversePalindrome.com
*/


#include "ComponentParser.hpp"

#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <sstream>
#include <fstream>


ComponentParser::ComponentParser(Entities& entities, ResourceManager& resourceManager, b2World& world) :
	entities(entities),
	world(world)
{
	componentParsers["Controllable"] = [this](auto& entity, const auto& line)
	{
		entity.set_tag<Controllable>(true);
	};
	
	componentParsers["Position1"] = [this](auto& entity, const auto& line)
	{
		entity.add_component<PositionComponent>();
	};

	componentParsers["Position2"] = [this](auto& entity, const auto& line) 
	{
		std::istringstream iStream(line);
		float xPosition = 0.f, yPosition = 0.f;

		iStream >> xPosition >> yPosition;

		entity.add_component<PositionComponent>(sf::Vector2f(xPosition, yPosition));
	};

	componentParsers["State"] = [this](auto& entity, const auto& line)
	{
		entity.add_component<StateComponent>();
	};

	componentParsers["Physics"] = [this, &world](auto& entity, const auto& line)
	{
		std::stringstream iStream(line);
		
		float sizeX = 0.f, sizeY = 0.f, positionX = 0.f, positionY = 0.f, maxVelocity = 0.f, accelerationRate = 0.f;
		std::size_t objectType = 0u;

		iStream >> sizeX >> sizeY >> positionX >> positionY >> maxVelocity >> accelerationRate >> objectType;

		entity.add_component<PhysicsComponent>(world, b2Vec2(sizeX, sizeY), b2Vec2(positionX, positionY), maxVelocity, accelerationRate, static_cast<ObjectType>(objectType));
	};

	componentParsers["AI"] = [this](auto& entity, const auto& line)
	{
		std::stringstream iStream(line);

		float visionRange = 0.f, attackTime = 0.f, pathPointA = 0.f, pathPointB = 0.f;

		iStream >> visionRange >> attackTime >> pathPointA >> pathPointB;

		entity.add_component<AIComponent>(visionRange, attackTime, std::make_pair(pathPointA, pathPointB));
	};

	componentParsers["Sprite1"] = [this, &resourceManager](auto& entity, const auto& line)
	{
		std::istringstream iStream(line);

		std::size_t textureID = 0u;

		iStream >> textureID;

		entity.add_component<SpriteComponent>(resourceManager.getTexture(static_cast<TexturesID>(textureID)));
	};

	componentParsers["Sprite2"] = [this, &resourceManager](auto& entity, const auto& line)
	{
		std::istringstream iStream(line);

		std::size_t textureID = 0u, left = 0u, top = 0u, width = 0u, height = 0u;

		iStream >> textureID >> left >> top >> width >> height;

		entity.add_component<SpriteComponent>(resourceManager.getTexture(static_cast<TexturesID>(textureID)), sf::IntRect(left, top, width, height));
	};
		
	componentParsers["Sprite3"] = [this, &resourceManager](auto& entity, const auto& line)
	{
		std::istringstream iStream(line);

		std::size_t textureID = 0u, left = 0u, top = 0u, width = 0u, height = 0u;

		float scaleX = 0.f, scaleY = 0.f;
		
		iStream >> textureID >> left >> top >> width >> height >> scaleX >> scaleY;
		
		entity.add_component<SpriteComponent>(resourceManager.getTexture(static_cast<TexturesID>(textureID)),
			sf::IntRect(left, top, width, height), sf::Vector2f(scaleX, scaleY));
	};
	
	componentParsers["Health"] = [this](auto& entity, const auto& line)
	{
		std::istringstream iStream(line);

		std::size_t hitpoints = 0u;

		iStream >> hitpoints;

		entity.add_component<HealthComponent>(hitpoints);
	};

	componentParsers["Attack"] = [this](auto& entity, const auto& line)
	{
		std::istringstream iStream(line);

		std::size_t damagePoints = 0u;

		iStream >> damagePoints;

		entity.add_component<AttackComponent>(damagePoints);
	};

	componentParsers["Animation"] = [this](auto& entity, const auto& line)
	{
		std::istringstream iStream(line);

		std::string animationFile;

		iStream >> animationFile;

		entity.add_component<AnimationComponent>(animationFile);
	};

	componentParsers["Sound"] = [this](auto& entity, const auto& line)
	{
		entity.add_component<SoundComponent>();
	};

	componentParsers["Particle"] = [this, &resourceManager](auto& entity, const auto& line)
	{
		std::istringstream iStream(line);

		std::string particlesFile;

		iStream >> particlesFile;
		
		entity.add_component<ParticleComponent>(particlesFile, resourceManager);
	};
}

void ComponentParser::parseComponents(const std::string& filePath)
{
	auto& entity = entities.create_entity();
	
	std::ifstream inFile(filePath);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string component;
	   
		iStream >> component;

		line.erase(std::begin(line), std::begin(line) + component.size());
		boost::remove_erase_if(line, boost::is_any_of(",()"));
		
		this->componentParsers[component](entity, line);
	}
}