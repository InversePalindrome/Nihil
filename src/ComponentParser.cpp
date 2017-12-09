/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentParser.cpp
InversePalindrome.com
*/


#include "ComponentParser.hpp"
#include "FilePaths.hpp"
#include "UnitConverter.hpp"
#include "EntityUtility.hpp"
#include "MathUtility.hpp"

#include <boost/filesystem/operations.hpp>
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

	componentParsers["Turret"] = [this](auto& entity, auto& line)
	{
		entity.set_tag<Turret>(true);
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
		const auto& [bodySizeX, bodySizeY, bodyType, objectType, maxVelocityX, maxVelocityY, accelerationX, accelerationY, jumpVelocity] 
			= parse<float, float, std::size_t, std::size_t, float, float, float, float, float>(line);

		entity.add_component<PhysicsComponent>(world, b2Vec2(bodySizeX, bodySizeY),
			static_cast<b2BodyType>(bodyType), static_cast<ObjectType>(objectType),
			b2Vec2(maxVelocityX, maxVelocityY), b2Vec2(accelerationX, accelerationY), jumpVelocity);
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
		const auto& [textureID, scaleX, scaleY] = parse<std::size_t, float, float>(line);

		entity.add_component<SpriteComponent>(resourceManager, TexturesID{ textureID },
			sf::Vector2f(scaleX, scaleY));
	};

	componentParsers["SpriteB"] = [this, &resourceManager](auto& entity, auto& line)
	{
		const auto& [textureID, left, top, width, height, scaleX, scaleY] 
			= parse<std::size_t , std::size_t, std::size_t, std::size_t, std::size_t, float, float> (line);

		entity.add_component<SpriteComponent>(resourceManager, TexturesID{ textureID },
			sf::IntRect(left, top, width, height), sf::Vector2f(scaleX, scaleY));
	};

	componentParsers["SpriteC"] = [this, &resourceManager](auto& entity, auto& line)
	{
		const auto& [fileName] = parse<std::string>(line);

		entity.add_component<SpriteComponent>(resourceManager, fileName);
	};

	componentParsers["Text"] = [this, &resourceManager](auto& entity, auto& line)
	{
		const auto& [inputText, fileName] = parse<std::string, std::string>(line);

		entity.add_component<TextComponent>(resourceManager, inputText, fileName);
	};

	componentParsers["Dialog"] = [this, &resourceManager](auto& entity, auto& line)
	{
		const auto& [dialogueTime, dialogFile, textStyleFile, spriteFile, textOffsetX, textOffsetY, offsetX, offsetY]
			= parse<float, std::string, std::string, std::string, float, float, float, float>(line);

		entity.add_component<DialogComponent>(resourceManager, dialogueTime,
			dialogFile, textStyleFile, spriteFile, sf::Vector2f(textOffsetX, textOffsetY),
			sf::Vector2f(offsetX, offsetY));
	};

	componentParsers["Health"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<HealthComponent>(parse<std::int32_t>(line)));
	};

	componentParsers["MeleeA"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<MeleeAttackComponent>(parse<std::int32_t>(line)));
	};

	componentParsers["MeleeB"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<MeleeAttackComponent>(parse<std::int32_t, float>(line)));
	};

	componentParsers["Range"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<RangeAttackComponent>(parse<std::string, float, float>(line)));
	};

	componentParsers["Bullet"] = [this](auto& entity, auto& line)
	{
		const auto& [damagePoints, soundID, force] = this->parse<std::int32_t, std::size_t, float>(line);

		entity.add_component<BulletComponent>(damagePoints, SoundBuffersID{ soundID }, force);
	};

	componentParsers["Bomb"] = [this](auto& entity, auto& line)
	{
		const auto& [damagePoints, soundID, explosionTime, explosionKnockback, explosionID] 
			= this->parse<std::int32_t, std::size_t, float, float, std::string>(line);

		entity.add_component<BombComponent>(damagePoints, SoundBuffersID{ soundID }, explosionTime, explosionKnockback, explosionID);
	};

	componentParsers["Animation"] = [this](auto& entity, auto& line)
	{
		entity.add_component(std::make_from_tuple<AnimationComponent>(parse< std::string>(line)));
	};

	componentParsers["Particle"] = [this, &resourceManager](auto& entity, auto& line)
	{
		const auto& [effectRangeX, effectRangeY, particleFile, emitterFile] = parse<float, float, std::string, std::string>(line);

		entity.add_component<ParticleComponent>(resourceManager, sf::Vector2f(effectRangeX, effectRangeY), particleFile, emitterFile);
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
		entity.add_component<AutomatedComponent>();
	};

	componentParsers["Pickup"] = [this](auto& entity, auto& line)
	{
		const auto& [itemID, soundID] = this->parse<std::size_t, std::size_t>(line);

		entity.add_component<PickupComponent>(Item{ itemID }, SoundBuffersID{ soundID });
	};

	componentParsers["PowerUp"] = [this](auto& entity, auto& line)
	{
		const auto& [itemID, soundID, effectTime, effectBoost] = this->parse<std::size_t, std::size_t, float, float>(line);

		entity.add_component<PowerUpComponent>(Item{ itemID }, SoundBuffersID{ soundID }, effectTime, effectBoost);
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

Entity ComponentParser::parseEntity(std::int32_t entityType, const std::string& fileName)
{
	return this->parseComponents(++this->currentEntityID * entityType, fileName);
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

		Utility::setPosition(entity, sf::Vector2f(xPosition, yPosition));
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

void ComponentParser::copyBlueprint(const std::string& fileName, const std::string& copiedFileName)
{
	if (!boost::filesystem::exists(Path::blueprints / copiedFileName))
	{
		boost::filesystem::copy_file(Path::blueprints / fileName, Path::blueprints / copiedFileName);
	}
}

Entity ComponentParser::createEntity()
{
	return this->entities.create_entity();
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

		if (this->componentParsers.count(componentName))
		{
			this->componentParsers[componentName](entity, line);
		}
	}

	this->setComponentsID(entity, entityID);

	if (this->currentEntityID < std::abs(entityID))
	{
		this->currentEntityID = std::abs(entityID);
	}

	return entity;
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