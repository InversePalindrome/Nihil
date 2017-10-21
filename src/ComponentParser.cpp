/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentParser.cpp
InversePalindrome.com
*/


#include "ComponentParser.hpp"
#include "FilePaths.hpp"
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
	componentParsers["AI"] = [this](auto& entity, auto& line)
	{
		entity.set_tag<AI>(true);
	};

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
		entity.add_component(std::make_from_tuple<TimerComponent>(parse<std::string>(line)));
	};

	componentParsers["PositionA"] = [this](auto& entity, auto& line) 
	{
		entity.add_component(std::make_from_tuple<PositionComponent>(parse<float, float>(line)));
	};

	componentParsers["PositionB"] = [this](auto& entity, auto& line)
	{
		entity.add_component<PositionComponent>();
	};

	componentParsers["State"] = [this](auto& entity, const auto& line)
	{
		entity.add_component<StateComponent>();
	};

	componentParsers["Physics"] = [this, &world](auto& entity, auto& line)
	{
		auto& params = parse<float, float, std::size_t, std::size_t, std::int32_t, float, float, float>(line);

		entity.add_component<PhysicsComponent>(world, b2Vec2(std::get<0>(params), std::get<1>(params)),
			static_cast<b2BodyType>(std::get<2>(params)), static_cast<ObjectType>(std::get<3>(params)), std::get<4>(params),
			std::get<5>(params), std::get<6>(params), std::get<7>(params));
	};

	componentParsers["Patrol"] = [this](auto& entity, auto& line)
	{
		entity.add_component<PatrolComponent>();
	};

	componentParsers["Chase"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<ChaseComponent>(parse<float>(line)));
	};

	componentParsers["SpriteA"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = parse<std::size_t, float, float>(line);

		entity.add_component<SpriteComponent>(resourceManager, static_cast<TexturesID>(std::get<0>(params)),
			sf::Vector2f(std::get<1>(params), std::get<2>(params)));
	};

	componentParsers["SpriteB"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = parse<std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, float, float>(line);

		entity.add_component<SpriteComponent>(resourceManager, static_cast<TexturesID>(std::get<0>(params)),
			sf::IntRect(std::get<1>(params), std::get<2>(params), std::get<3>(params), std::get<4>(params)), sf::Vector2f(std::get<5>(params), std::get<6>(params)));
	};

	componentParsers["SpriteC"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = parse<std::string>(line);

		entity.add_component<SpriteComponent>(resourceManager, std::get<0>(params));
	};

	componentParsers["Text"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = parse<std::string, std::string>(line);

		entity.add_component<TextComponent>(resourceManager, std::get<0>(params), std::get<1>(params));
	};

	componentParsers["Dialog"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = parse<float, std::string, std::string, std::string, float, float, float, float>(line);

		entity.add_component<DialogComponent>(resourceManager, std::get<0>(params),
			std::get<1>(params), std::get<2>(params), std::get<3>(params), sf::Vector2f(std::get<4>(params), std::get<5>(params)),
			sf::Vector2f(std::get<6>(params), std::get<7>(params)));
	};

	componentParsers["Health"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<HealthComponent>(parse<std::size_t>(line)));
	};

	componentParsers["MeleeA"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<MeleeAttackComponent>(parse<std::size_t>(line)));
	};

	componentParsers["MeleeB"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<MeleeAttackComponent>(parse<std::size_t, float>(line)));
	};

	componentParsers["Range"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<RangeAttackComponent>(parse<std::string, float, float>(line)));
	};

	componentParsers["Bullet"] = [this](auto& entity, auto& line)
	{
		auto& params = this->parse<std::size_t, std::size_t, float>(line);

		entity.add_component<BulletComponent>(std::get<0>(params), static_cast<SoundBuffersID>(std::get<1>(params)), std::get<2>(params));
	};

	componentParsers["Bomb"] = [this](auto& entity, auto& line)
	{
		auto& params = this->parse<std::size_t, std::size_t, float, float, std::string>(line);

		entity.add_component<BombComponent>(std::get<0>(params), static_cast<SoundBuffersID>(std::get<1>(params)), std::get<2>(params), std::get<3>(params), std::get<4>(params));
	};

	componentParsers["Animation"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<AnimationComponent>(parse<bool, std::string>(line)));
	};

	componentParsers["Sound"] = [this](auto& entity, auto& line)
	{
		entity.add_component<SoundComponent>();
	};

	componentParsers["Particle"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = parse<float, float, std::string, std::string>(line);
		
		entity.add_component<ParticleComponent>(resourceManager, sf::Vector2f(std::get<0>(params), std::get<1>(params)), std::get<2>(params), std::get<3>(params));
	};
	
	componentParsers["ParentA"] = [this](auto& entity, auto& line)
	{
		entity.add_component<ParentComponent>();
	};

	componentParsers["ParentB"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<ParentComponent>(parse<std::int32_t>(line)));
	};

	componentParsers["ChildA"] = [this](auto& entity, auto& line)
	{
		entity.add_component<ChildComponent>();
	};

	componentParsers["ChildB"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<ChildComponent>(parse<std::int32_t>(line)));
	};

	componentParsers["Automated"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<AutomatedComponent>(parse<std::string>(line)));
	};

	componentParsers["Pickup"] = [this](auto& entity, auto& line)
	{
		auto& params = this->parse<std::size_t, std::size_t>(line);

		entity.add_component<PickupComponent>(static_cast<Item>(std::get<0>(params)), static_cast<SoundBuffersID>(std::get<1>(params)));
	};

	componentParsers["PowerUp"] = [this](auto& entity, auto& line)
	{
		auto& params = this->parse<std::size_t, std::size_t, float, float>(line);

		entity.add_component<PowerUpComponent>(static_cast<Item>(std::get<0>(params)), static_cast<SoundBuffersID>(std::get<1>(params)), std::get<2>(params), std::get<3>(params));
	};

	componentParsers["Drop"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<DropComponent>(parse<std::string>(line)));
	};

	componentParsers["Inventory"] = [this](auto& entity, auto& line)
	{
		entity.add_component<InventoryComponent>();
	};

	componentParsers["Lock"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<LockComponent>(parse<std::size_t, std::string>(line)));
	}; 
	
	componentParsers["Key"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<KeyComponent>(parse<std::size_t>(line)));
	};
}

Entity ComponentParser::parseComponents(const std::string& fileName)
{
	return this->parseComponents(++this->currentEntityID, fileName);
}

Entity ComponentParser::parseComponents(std::int32_t entityID, const std::string& fileName)
{
	auto entity = this->createEntity();

	std::ifstream inFile(Path::blueprints / fileName);
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

	this->setComponentsID(entity, entityID);

	if (this->currentEntityID < std::abs(entityID))
	{
		this->currentEntityID = std::abs(entityID);
	}

	return entity;
}

void ComponentParser::parseBlueprint(const std::string& fileName)
{
	std::ifstream inFile(Path::blueprints / fileName);
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
			entity.get_component<PositionComponent>().setDialoguePosition(sf::Vector2f(xPosition, yPosition));
		}
		if (entity.has_component<PhysicsComponent>())
		{
			entity.get_component<PhysicsComponent>().setDialoguePosition(
				b2Vec2(UnitConverter::pixelsToMeters(xPosition), UnitConverter::pixelsToMeters(-yPosition)));
		}
	}
}

void ComponentParser::parseEntities(const std::string& fileName)
{
	std::ifstream inFile(Path::games / fileName);
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
		this->setComponentsID(entity.second, entity.first);
	}

	if (!entitiesIDs.empty())
	{
		this->currentEntityID = std::rbegin(entitiesIDs)->first;
	}
}

Entity ComponentParser::createEntity()
{
	return this->entities.create_entity();
}

void ComponentParser::setComponentsID(Entity entity, std::int32_t entityID)
{
	brigand::for_each<ComponentList>([entity, entityID](auto componentType) mutable
	{
		using Type = decltype(componentType)::type;

		if (entity.has_component<Type>())
		{
			entity.get_component<Type>().setEntityID(entityID);
		}
	});
}
