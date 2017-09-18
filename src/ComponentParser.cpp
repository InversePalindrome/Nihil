/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentParser.cpp
InversePalindrome.com
*/


#include "ComponentParser.hpp"
#include "UnitConverter.hpp"

#include <brigand/algorithms/for_each.hpp>

#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <map>
#include <fstream>


ComponentParser::ComponentParser(Entities& entities, ResourceManager& resourceManager, b2World& world) :
	entities(entities),
	world(world),
	currentEntityID(0)
{
	componentParsers["Controllable"] = [this](auto& entity, auto& line)
	{
		entity.add_component<ControllableComponent>();
	};

	componentParsers["TimerA"] = [this](auto& entity, auto& line)
	{
		entity.add_component<TimerComponent>();
	};

	componentParsers["TimerB"] = [this](auto& entity, auto& line)
	{
		entity.add_component<TimerComponent>(std::make_from_tuple<TimerComponent>(parse<std::string>(line)));
	};

	componentParsers["PositionA"] = [this](auto& entity, auto& line) 
	{
		entity.add_component<PositionComponent>(std::make_from_tuple<PositionComponent>(parse<float, float>(line)));
	};

	componentParsers["PositionB"] = [this](auto& entity, auto& line)
	{
		entity.add_component<PositionComponent>();
	};


	componentParsers["State"] = [this](auto& entity, const auto& line)
	{
		entity.add_component<StateComponent>();
	};

	componentParsers["PhysicsA"] = [this, &world](auto& entity, auto& line)
	{
		auto& params = parse<float, float, std::size_t, std::size_t, std::int32_t, float, float>(line);

		entity.add_component<PhysicsComponent>(world, b2Vec2(std::get<0>(params), std::get<1>(params)),
			static_cast<b2BodyType>(std::get<2>(params)), static_cast<ObjectType>(std::get<3>(params)), std::get<4>(params), std::get<5>(params), std::get<6>(params));
	};

	componentParsers["PhysicsB"] = [this, &world](auto& entity, auto& line)
	{
		auto& params = parse<float, float, std::size_t, std::size_t, std::int32_t>(line);

		entity.add_component<PhysicsComponent>(world, b2Vec2(std::get<0>(params), std::get<1>(params)),
			static_cast<b2BodyType>(std::get<2>(params)), static_cast<ObjectType>(std::get<3>(params)), std::get<4>(params));
	};

	componentParsers["Patrol"] = [this](auto& entity, auto& line)
	{
		entity.add_component<PatrolComponent>();
	};

	componentParsers["Chase"] = [this](auto& entity, auto& line)
	{
		entity.add_component<ChaseComponent>(std::make_from_tuple<ChaseComponent>(parse<float>(line)));
	};

	componentParsers["SpriteA"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = parse<std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, float, float>(line);

		entity.add_component<SpriteComponent>(resourceManager, static_cast<TexturesID>(std::get<0>(params)),
			sf::IntRect(std::get<1>(params), std::get<2>(params), std::get<3>(params), std::get<4>(params)), sf::Vector2f(std::get<5>(params), std::get<6>(params)));
	};

	componentParsers["SpriteB"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = parse<std::size_t, std::size_t, std::size_t, std::size_t, std::size_t>(line);

		entity.add_component<SpriteComponent>(resourceManager, static_cast<TexturesID>(std::get<0>(params)),
			sf::IntRect(std::get<1>(params), std::get<2>(params), std::get<3>(params), std::get<4>(params)));
	};

	componentParsers["SpriteC"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = parse<std::size_t>(line);

		entity.add_component<SpriteComponent>(resourceManager, static_cast<TexturesID>(std::get<0>(params)));
	};

	componentParsers["Health"] = [this](auto& entity, auto& line)
	{
		entity.add_component<HealthComponent>(std::make_from_tuple<HealthComponent>(parse<std::size_t>(line)));
	};

	componentParsers["Melee"] = [this](auto& entity, auto& line)
	{
		entity.add_component<MeleeAttackComponent>(std::make_from_tuple<MeleeAttackComponent>(parse<std::size_t>(line)));
	};

	componentParsers["Range"] = [this](auto& entity, auto& line)
	{
		entity.add_component<RangeAttackComponent>(std::make_from_tuple<RangeAttackComponent>(parse<std::size_t, float, float>(line)));
	};

	componentParsers["Animation"] = [this](auto& entity, auto& line)
	{
		entity.add_component<AnimationComponent>(std::make_from_tuple<AnimationComponent>(parse<std::string>(line)));
	};

	componentParsers["Sound"] = [this](auto& entity, auto& line)
	{
		entity.add_component<SoundComponent>();
	};

	componentParsers["Particle"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = this->parse<std::string>(line);
		
		entity.add_component<ParticleComponent>(std::get<0>(params), resourceManager);
	};
	
	componentParsers["Parent"] = [this](auto& entity, auto& line)
	{
		entity.add_component<ParentComponent>(std::make_from_tuple<ParentComponent>(parse<std::size_t>(line)));
	};

	componentParsers["Child"] = [this](auto& entity, auto& line)
	{
		entity.add_component<ChildComponent>(std::make_from_tuple<ChildComponent>(parse<std::size_t>(line)));
	};

	componentParsers["Automated"] = [this](auto& entity, auto& line)
	{
		entity.add_component<AutomatedComponent>(std::make_from_tuple<AutomatedComponent>(parse<std::string>(line)));
	};
}

Entity ComponentParser::parseComponents(const std::string& pathFile)
{
	return this->parseComponents(++this->currentEntityID, pathFile);
}

Entity ComponentParser::parseComponents(std::int32_t entityID, const std::string& pathFile)
{
	auto entity = this->createEntity();
	
	std::ifstream inFile(pathFile);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string componentName;

		iStream >> componentName;

		line.erase(std::begin(line), std::begin(line) + componentName.size());
		boost::remove_erase_if(line, boost::is_any_of(",()"));
		
		this->componentParsers[componentName](entity, line);
	}

	brigand::for_each<ComponentList>([&entity, entityID, this](auto componentType)
	{
		using Type = decltype(componentType)::type;

		if (entity.has_component<Type>())
		{
			entity.get_component<Type>().setEntityID(entityID);
		}
	});

	if (this->currentEntityID < std::abs(entityID))
	{
		this->currentEntityID = std::abs(entityID);
	}

	return entity;
}

void ComponentParser::parseBlueprint(const std::string& pathFile)
{
	std::ifstream inFile(pathFile);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::int32_t entityID = 0;
		std::string entityFile;
		float xPosition = 0.f, yPosition = 0.f;

		iStream >> entityID >> entityFile >> xPosition >> yPosition;
		
		auto entity = this->parseComponents(entityID, entityFile);

		if (entity.has_component<PositionComponent>())
		{
			entity.get_component<PositionComponent>().setPosition(sf::Vector2f(xPosition, yPosition));
		}
		if (entity.has_component<PhysicsComponent>())
		{
			entity.get_component<PhysicsComponent>().setPosition(
				b2Vec2(UnitConverter::pixelsToMeters(xPosition), UnitConverter::pixelsToMeters(-yPosition)));
		}
	}
}

void ComponentParser::parseEntities(const std::string& pathFile)
{
	std::ifstream inFile(pathFile);
	std::string line;

	std::map<std::int32_t, Entity> entitiesIDs;
	std::int32_t entityID = 0;

	while (std::getline(inFile, line))
	{
		if (line.empty())
		{
			continue;
		}

		std::istringstream iStream(line);

		std::string category;

		iStream >> category;

		if (category == "Entity")
		{
			iStream >> entityID;

			entitiesIDs.emplace(entityID, this->createEntity());
		}
		else
		{
   			line.erase(std::begin(line), std::begin(line) + category.size());
			
			this->componentParsers[category](entitiesIDs[entityID], line);
		}
	}

	for (auto& entity : entitiesIDs)
	{
		brigand::for_each<ComponentList>([&entity, this](auto componentType)
		{
			entity.second.sync();
			
			using Type = decltype(componentType)::type;
			
			if (entity.second.has_component<Type>())
			{
				entity.second.get_component<Type>().setEntityID(entity.first);

				if (this->currentEntityID < std::abs(entity.first))
				{
					this->currentEntityID = std::abs(entity.first);
				}
			}
		});
	}
}

Entity ComponentParser::createEntity()
{
	return this->entities.create_entity();
}