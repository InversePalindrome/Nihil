/*
Copyright (c) 2017 InversePalindrome
Nihil - GameState.cpp
InversePalindrome.com
*/


#include "GameState.hpp"
#include "StateMachine.hpp"
#include "FilePaths.hpp"
#include "UnitConverter.hpp"


GameState::GameState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	world({ 0.f, -9.8f }),
	entityManager(world, stateData.resourceManager, stateData.soundManager, stateData.inputHandler, collisionsData, pathways),
	map(stateData.games.front(), world, entityManager.getComponentSerializer(), stateData.resourceManager, collisionsData, pathways),
	camera(stateData.window.getDefaultView()),
	collisionHandler(entityManager.getEvents()),
	healthBar(stateData.resourceManager),
	coinDisplay(stateData.resourceManager),
	itemsDisplay(stateData.resourceManager),
	powerUpDisplay(stateData.resourceManager)
{
	world.SetContactListener(&collisionHandler);
	
	healthBar.setPosition(150.f, 120.f);

	coinDisplay.setPosition(1600.f, 120.f);
	coinDisplay.setNumberOfCoins(stateData.games.front().getItems()[Item::Coin]);

	itemsDisplay.setPosition(200.f, 200.f);

	powerUpDisplay.setPosition(600.f, 100.f);
	
	entityManager.getEvents().subscribe<DisplayHealthBar>([this](const auto& event) { updateHealthBar(event.health); });
	entityManager.getEvents().subscribe<DisplayCoins>([this](const auto& event) { updateCoinDisplay(event.inventory); });
	entityManager.getEvents().subscribe<PickedUpItem>([this](const auto& event) { updateItemsDisplay(event.item); });
	entityManager.getEvents().subscribe<DisplayPowerUp>([this](const auto& event) { powerUpDisplay.addPowerUp(event.item); });
	entityManager.getEvents().subscribe<HidePowerUp>([this](const auto& event) { powerUpDisplay.removePowerUp(event.item); });

	entityManager.getEvents().subscribe<CreateEntity>([this](const auto& event) 
	{ 
		callbacks.addCallback([this, event]()
		{
			entityManager.createEntity(event.fileName, event.position);
		});
	});

	entityManager.getEvents().subscribe<DestroyEntity>([this](const auto& event) 
	{ 
		callbacks.addCallback([this, event]()
		{ 
		    entityManager.destroyEntity(event.entity);
		}); 

		if (event.entity.has_component<ControllableComponent>())
		{
			entityManager.getEvents().broadcast(PlayerDied{});
		}
	});

	entityManager.getEvents().subscribe<PlayerDied>([this, &stateData](const auto& event)
	{
		callbacks.addCallback([this, &stateData]
		{
			entityManager.createEntity("Player.txt", stateData.games.front().getCurrentSpawnPoint());
			
			moveCamera(stateData.games.front().getCurrentSpawnPoint());
		});
	});

	entityManager.getEvents().subscribe<ChangePosition>([this](const auto& event)
	{
		changeEntityPosition(event.entity, event.location);
		moveCamera(event.location);
	});

	entityManager.getEvents().subscribe<ChangeLevel>([this, &stateData](const auto& event)
	{
		callbacks.addCallback([this, event]
		{ 
			changeLevel(event.level);
		});

		this->saveData("SavedGames.txt");
	});

    entityManager.getEvents().subscribe<entityplus::component_added<Entity, InventoryComponent>>([this, &stateData](auto& event)
	{
		event.component.setItems(stateData.games.front().getItems());

		for (const auto& item : stateData.games.front().getItems())
		{
			itemsDisplay[item.first].quantity = item.second;
			itemsDisplay[item.first].info.setString(std::to_string(item.second) + " / " + std::to_string(itemsDisplay[item.first].maxQuantity));
		}
	});

	changeLevel(stateData.games.front().getCurrentLevel());
}

void GameState::handleEvent(const sf::Event& event)
{
	if (this->stateData.inputHandler.isActive("Escape"))
	{
		this->saveData("SavedGames.txt");
		this->stateMachine.pushState(StateID::Pause);
	}
	else if (this->stateData.inputHandler.isActive("Inventory"))
	{
		this->itemsDisplay.setVisibility(!this->itemsDisplay.getVisibility());
	}
}

void GameState::update(float deltaTime)
{
	const float timeStep = 1.f / 60.f;
	const std::size_t velocityIterations = 6u;
	const std::size_t positionIterations = 2u;

	this->world.Step(timeStep, velocityIterations, positionIterations);
	
	this->updateCamera();

	this->entityManager.update(deltaTime);
	this->coinDisplay.update(deltaTime);
	this->itemsDisplay.update(deltaTime);
	this->powerUpDisplay.update(deltaTime);

	this->callbacks.update();
	this->callbacks.clear();
}

void GameState::draw()
{
	this->stateData.window.setView(this->camera);
	this->stateData.window.draw(this->map);
	this->stateData.window.draw(this->entityManager);

	this->stateData.window.setView(this->stateData.window.getDefaultView());
	this->stateData.window.draw(this->healthBar);
	this->stateData.window.draw(this->coinDisplay);
	this->stateData.window.draw(this->itemsDisplay);
	this->stateData.window.draw(this->powerUpDisplay);
}

void GameState::moveCamera(const sf::Vector2f& position)
{
	this->camera.setCenter(this->stateData.window.getDefaultView().getCenter());

	switch (this->stateData.games.front().getCurrenDirectionType())
	{
	case DirectionType::Horizontal:
		this->camera.setCenter(position.x, this->camera.getCenter().y);
		break;
	case DirectionType::Vertical:
		this->camera.setCenter(this->camera.getCenter().x, position.y);
		break;
	}
}

void GameState::updateCamera()
{
	this->entityManager.getEntities().for_each<ControllableComponent, PositionComponent>([this](auto entity, auto& controllable, auto& position)
	{
		const auto& centerPosition = position.getPosition();

		switch (this->stateData.games.front().getCurrenDirectionType())
		{
		case DirectionType::Horizontal:
		{
			if (centerPosition.x > this->camera.getSize().x / 2.f && centerPosition.x < this->map.getBounds().width - this->camera.getSize().x / 2.f)
			{
				this->camera.setCenter(centerPosition.x, this->camera.getCenter().y);
			}
		}
		break;
		case DirectionType::Vertical:
		{
			if (centerPosition.y > this->camera.getSize().y / 2.f && centerPosition.y < this->map.getBounds().height - this->camera.getSize().y / 2.f)
			{
				this->camera.setCenter(this->camera.getCenter().x, centerPosition.y);
			}
		}
		break;
		}
	});
}

void GameState::updateHealthBar(const HealthComponent& healthComponent)
{
	this->healthBar.setHitpointsDisplay(healthComponent.getHitpoints());
}

void GameState::updateCoinDisplay(const InventoryComponent& inventory)
{
	this->coinDisplay.setNumberOfCoins(inventory[Item::Coin]);
}

void GameState::updateItemsDisplay(Entity item)
{
	if (item.has_component<PickupComponent>() && this->itemsDisplay.hasItem(item.get_component<PickupComponent>().getItem()))
	{
		auto& itemGraphic = this->itemsDisplay[item.get_component<PickupComponent>().getItem()];

		++itemGraphic.quantity;
		itemGraphic.info.setString(std::to_string(itemGraphic.quantity) + " / " + std::to_string(itemGraphic.maxQuantity));
	}
}

void GameState::changeLevel(const std::string& level)
{
	this->stateData.games.front().setCurrentLevel(level);

	this->entityManager.destroyEntities();
	
	this->map.load(level + ".tmx");
	
	if (!this->stateData.games.front().getLevels().get<1>().find(level)->isLoaded)
	{
		this->stateData.games.front().getLevels().get<1>().modify(this->stateData.games.front().getLevels().get<1>().find(level), [](auto& iLevel) { iLevel.isLoaded = true; });
		this->entityManager.parseBlueprint("Objects-" + level + ".txt");
	}
	else
	{
		this->entityManager.parseEntities(this->stateData.games.front().getGameName() + '-' + level + ".txt");
	}

	this->entityManager.parseBlueprint("Entities-" + level + ".txt");
	this->stateData.soundManager.stopAllSounds();
	this->powerUpDisplay.clearPowerUps();

	this->entityManager.getEntities().for_each<ControllableComponent, PositionComponent, PhysicsComponent>([this](auto entity, auto& controllable, auto& position, auto& physics)
	{
		const auto& spawnPoint = this->stateData.games.front().getCurrentSpawnPoint();

		position.setPosition({ spawnPoint.x, spawnPoint.y });
		physics.setPosition({ UnitConverter::pixelsToMeters(spawnPoint.x), UnitConverter::pixelsToMeters(-spawnPoint.y) });
		
		this->moveCamera(spawnPoint);
	});

	this->world.SetGravity(this->stateData.games.front().getCurrentGravity());
}

void GameState::changeEntityPosition(Entity entity, const sf::Vector2f& position)
{
	if (entity.has_component<PositionComponent>())
	{
		entity.get_component<PositionComponent>().setPosition(position);
	}
	if (entity.has_component<PhysicsComponent>())
	{
		this->callbacks.addCallback([entity, position]() mutable
		{
			entity.get_component<PhysicsComponent>().setPosition({ UnitConverter::pixelsToMeters(position.x), UnitConverter::pixelsToMeters(-position.y) });
		});
	}
}

void GameState::saveData(const std::string& fileName)
{
	this->entityManager.saveEntities(this->stateData.games.front().getGameName() + '-' + this->stateData.games.front().getCurrentLevel() + ".txt");
	
	std::ofstream outFile(Path::games / fileName);
	
	for (const auto& game : this->stateData.games)
	{
		outFile << game << '\n';
	}
}