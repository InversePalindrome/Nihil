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
	entityManager(world, stateData.resourceManager, stateData.soundManager, stateData.inputHandler, collisionsData),
	map(sf::Vector2f(8192.f, 1536.f), world, entityManager, stateData.resourceManager, collisionsData),
	camera(stateData.window.getDefaultView()),
	collisionHandler(entityManager.getEvents()),
	healthBar(stateData.resourceManager),
	coinDisplay(stateData.resourceManager)
{
	world.SetContactListener(&collisionHandler);

	healthBar.setPosition(150.f, 120.f);

	coinDisplay.setPosition(1600.f, 120.f);
	coinDisplay.setNumberOfCoins(stateData.player.getCoins());
	
	entityManager.getEvents().subscribe<DestroyEntity>([this](const auto& event) 
	{ 
		callbacks.push_back([this, event]
		{ 
		    entityManager.destroyEntity(event.entity);
		}); 
	});
	entityManager.getEvents().subscribe<FinishedLevel>([this, &stateData](const auto& event)
	{
		callbacks.push_back([this, &stateData]
		{ 
		    entityManager.destroyEntities();
			stateData.player.setCurrentLevel(stateData.player.getCurrentLevel() + 1u);
			changeLevel(stateData.player.getCurrentLevel());
		});
	});
	entityManager.getEvents().subscribe<GameOver>([this, &stateData](const auto& event)
	{
		callbacks.push_back([this, &stateData] 
		{
			entityManager.destroyEntities();
			changeLevel(stateData.player.getCurrentLevel());
		});
	});
	entityManager.getEvents().subscribe<PickedUpCoin>([this, &stateData](const auto& event)
	{
		stateData.player.setCoins(stateData.player.getCoins() + 1u);
		coinDisplay.setNumberOfCoins(stateData.player.getCoins());
	});
	entityManager.getEvents().subscribe<CombatOcurred>([this](const auto& event)
	{
		setDisplayHitpoints(event.victim);
	});

	stateData.window.setView(camera);

	changeLevel(stateData.player.getCurrentLevel());
}

void GameState::handleEvent(const sf::Event& event)
{
	if (this->stateData.inputHandler.isActive("Escape"))
	{
		this->stateMachine.pushState(StateID::Pause);
	}
	else if (this->stateData.inputHandler.isActive("Retry"))
	{
		this->entityManager.getEvents().broadcast(GameOver{});
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
	this->updateCamera();
	this->coinDisplay.update(deltaTime);
}

void GameState::draw()
{
	this->stateData.window.setView(this->camera);
	this->stateData.window.draw(this->map);
	this->stateData.window.draw(this->entityManager);

	this->stateData.window.setView(this->stateData.window.getDefaultView());

	this->stateData.window.draw(this->healthBar);
	this->stateData.window.draw(this->coinDisplay);
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
	this->map.load("Resources/Files/Level" + std::to_string(level) + ".tmx");
	this->entityManager.createEntities("Resources/Files/EntitiesLevel" + std::to_string(level) + ".txt");

	this->healthBar.setHitpointsDisplay(this->entityManager.getEntities().get_entities<Controllable, HealthComponent>()
		.back().get_component<HealthComponent>().getHitpoints());

	this->camera = this->stateData.window.getDefaultView();
	this->stateData.soundManager.stopAllSounds();
}

void GameState::setDisplayHitpoints(Entity entity)
{
	if (entity.has_tag<Controllable>())
	{
		this->healthBar.setHitpointsDisplay(entity.get_component<HealthComponent>().getHitpoints());
	}
}