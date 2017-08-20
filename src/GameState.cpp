/*
Copyright (c) 2017 InversePalindrome
Nihil - GameState.cpp
InversePalindrome.com
*/


#include "GameState.hpp"
#include "StateMachine.hpp"


GameState::GameState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	world(b2Vec2(0.f, -9.8f)),
	entityManager(stateData.events, world, stateData.resourceManager, stateData.soundManager, stateData.inputHandler, collisionsData),
	map(sf::Vector2f(8192.f, 1536.f), world, entityManager, stateData.resourceManager, collisionsData),
	camera(stateData.window.getDefaultView()),
	collisionHandler(entityManager.getEvents()),
	level(0u),
	gameHud(stateData.resourceManager)
{
	world.SetContactListener(&collisionHandler);

	entityManager.getEvents().subscribe<DestroyEntity>([this](const auto& event) 
	{ 
		callbacks.push_back([this, event]{ 
		entityManager.destroyEntity(event.entity); }); 
	});
	entityManager.getEvents().subscribe<FinishedLevel>([this](const auto& event)
	{
		callbacks.push_back([this]
		{ 
			gameHud.setCoinDisplayNumber(0u);
		    entityManager.destroyEntities();
		    changeLevel(level + 1u);
		});
	});
	entityManager.getEvents().subscribe<GameOver>([this](const auto& event)
	{
		callbacks.push_back([this] 
		{
			gameHud.setCoinDisplayNumber(0u);
			entityManager.destroyEntities();
			changeLevel(level);
		});
	});
	entityManager.getEvents().subscribe<PickedUpCoin>([this](const auto& event)
	{
		gameHud.addToCoinDisplayNumber();
	});
	entityManager.getEvents().subscribe<CombatOcurred>([this](const auto& event)
	{
		setDisplayHitpoints(event.victim);
	});

	stateData.window.setView(camera);

	changeLevel(level + 1u);
}

void GameState::handleEvent(const sf::Event& event)
{
	if (this->stateData.inputHandler.isActive("Escape"))
	{
		this->stateMachine.pushState(StateID::Pause);
	}
	else if (this->stateData.inputHandler.isActive("Retry"))
	{
		this->stateData.events.broadcast(GameOver{});
	}
}

void GameState::update(float deltaTime)
{
	const float timeStep = 1.f / 60.f;
	const std::size_t velocityIterations = 6u;
	const std::size_t positionIterations = 2u;

	this->world.Step(timeStep, velocityIterations, positionIterations);

	for (auto& callback : this->callbacks)
	{
		callback();
	}

	this->callbacks.clear();
	
	this->entityManager.update(deltaTime);
	this->gameHud.update(deltaTime);
	this->updateCamera();
}

void GameState::draw()
{
	this->stateData.window.setView(this->camera);
	this->stateData.window.draw(this->map);
	this->entityManager.draw(this->stateData.window);

	this->stateData.window.setView(this->stateData.window.getDefaultView());
	this->stateData.window.draw(this->gameHud);
}

void GameState::updateCamera()
{
	const auto& playerPosition = this->entityManager.getEntities().
		get_entities<Controllable, PositionComponent>().back().get_component<PositionComponent>().getPosition();
	
	if (playerPosition.x > this->camera.getSize().x / 2.f && playerPosition.x < this->map.getBounds().width - this->camera.getSize().x / 2.f)
	{
		this->camera.setCenter(playerPosition.x, this->camera.getCenter().y);
		this->stateData.window.setView(this->camera);
	}
}

void GameState::changeLevel(std::size_t level)
{
	this->level = level;

	this->map.load("Resources/Files/Level" + std::to_string(this->level) + ".tmx");
	this->entityManager.createEntities("Resources/Files/EntitiesLevel" + std::to_string(this->level) + ".txt");

	this->camera = this->stateData.window.getDefaultView();
	this->stateData.soundManager.stopAllSounds();

	this->gameHud.setNumberOfHitpoints(this->entityManager.getEntities().
		get_entities<Controllable, HealthComponent>().back().get_component<HealthComponent>().getHitpoints());
}

void GameState::setDisplayHitpoints(Entity entity)
{
	if (entity.has_tag<Controllable>())
	{
		this->gameHud.setNumberOfHitpoints(entity.get_component<HealthComponent>().getHitpoints());
	}
}