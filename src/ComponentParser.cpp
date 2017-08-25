/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentParser.cpp
InversePalindrome.com
*/


#include "ComponentParser.hpp"

#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <fstream>


ComponentParser::ComponentParser(Entities& entities, ResourceManager& resourceManager, b2World& world) :
	entities(entities),
	world(world)
{
	componentParsers["Controllable"] = [this](auto& entity, auto& line)
	{
		entity.set_tag<Controllable>(true);
	};

	componentParsers["Pickup"] = [this](auto& entity, auto& line)
	{
		entity.set_tag<Pickup>(true);
	};
	
	componentParsers["Position1"] = [this](auto& entity, auto& line)
	{
		entity.add_component<PositionComponent>();
	};

	componentParsers["Position2"] = [this](auto& entity, auto& line) 
	{
		entity.add_component<PositionComponent>(std::make_from_tuple<PositionComponent>(this->parse<float, float>(line)));
	};

	componentParsers["State"] = [this](auto& entity, const auto& line)
	{
		entity.add_component<StateComponent>();
	};

	componentParsers["Physics1"] = [this, &world](auto& entity, auto& line)
	{
		auto& params = this->parse<float, float, std::size_t, std::size_t, std::size_t>(line);

		entity.add_component<PhysicsComponent>(world, b2Vec2(std::get<0>(params), std::get<1>(params)), 0.f, 0.f,
			static_cast<b2BodyType>(std::get<2>(params)), static_cast<ObjectType>(std::get<3>(params)), std::get<4>(params));
	};

	componentParsers["Physics2"] = [this, &world](auto& entity, auto& line)
	{
		auto& params = this->parse<float, float, float, float, std::size_t, std::size_t, std::size_t>(line);

		entity.add_component<PhysicsComponent>(world, b2Vec2(std::get<0>(params), std::get<1>(params)), 
			std::get<2>(params), std::get<3>(params), static_cast<b2BodyType>(std::get<4>(params)), static_cast<ObjectType>(std::get<5>(params)), std::get<6>(params));
	};

	componentParsers["AI"] = [this](auto& entity, auto& line)
	{
		entity.add_component<AIComponent>(std::make_from_tuple<AIComponent>(this->parse<float, float, float, float>(line)));
	};

	componentParsers["Sprite1"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = this->parse<std::size_t>(line);

		entity.add_component<SpriteComponent>(resourceManager.getTexture(static_cast<TexturesID>(std::get<0>(params))));
	};

	componentParsers["Sprite2"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = this->parse<std::size_t, std::size_t, std::size_t, std::size_t, std::size_t>(line);

		entity.add_component<SpriteComponent>(resourceManager.getTexture(static_cast<TexturesID>(std::get<0>(params))),
			sf::IntRect(std::get<1>(params), std::get<2>(params), std::get<3>(params), std::get<4>(params)));
	};
		
	componentParsers["Sprite3"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = this->parse<std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, float, float>(line);
		
		entity.add_component<SpriteComponent>(resourceManager.getTexture(static_cast<TexturesID>(std::get<0>(params))),
			sf::IntRect(std::get<1>(params), std::get<2>(params), std::get<3>(params), std::get<4>(params)), sf::Vector2f(std::get<5>(params), std::get<6>(params)));
	};
	
	componentParsers["Health"] = [this](auto& entity, auto& line)
	{
		entity.add_component<HealthComponent>(std::make_from_tuple<HealthComponent>(this->parse<std::size_t>(line)));
	};

	componentParsers["Attack"] = [this](auto& entity, auto& line)
	{
		entity.add_component<AttackComponent>(std::make_from_tuple<AttackComponent>(this->parse<std::size_t>(line)));
	};

	componentParsers["Animation"] = [this](auto& entity, auto& line)
	{
		entity.add_component<AnimationComponent>(std::make_from_tuple<AnimationComponent>(this->parse<std::string>(line)));
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
		entity.add_component<ParentComponent>(std::make_from_tuple<ParentComponent>(this->parse<std::size_t>(line)));
	};

	componentParsers["Child"] = [this](auto& entity, auto& line)
	{
		entity.add_component<ChildComponent>(std::make_from_tuple<ChildComponent>(this->parse<std::size_t>(line)));
	};

	componentParsers["Automated"] = [this](auto& entity, auto& line)
	{
		auto& params = this->parse<std::string>(line);

		entity.add_component<AutomatedComponent>(std::get<0>(params));
	};
}

Entity ComponentParser::parseComponents(const std::string& filePath)
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

	return entity;
}