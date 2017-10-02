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
	entityManager(world, stateData.resourceManager, stateData.soundManager, stateData.inputHandler, collisionsData, pathways),
	map(sf::Vector2f(8192.f, 1536.f), stateData.games.front(), world, entityManager.getComponentSerializer(), stateData.resourceManager, collisionsData, pathways),
	camera(stateData.window.getDefaultView()),
	collisionHandler(entityManager.getEvents()),
	healthBar(stateData.resourceManager),
	coinDisplay(stateData.resourceManager),
	itemsDisplay(stateData.resourceManager)
{
	world.SetContactListener(&collisionHandler);

	healthBar.setPosition(150.f, 120.f);

	coinDisplay.setPosition(1600.f, 120.f);
	coinDisplay.setNumberOfCoins(stateData.games.front().getItems()[Item::Coin]);

	itemsDisplay.setPosition(200.f, 200.f);
	
	entityManager.getEvents().subscribe<MoveCamera>([this](const auto& event) { updateCamera(event.entity); });
	entityManager.getEvents().subscribe<DisplayHealthBar>([this](const auto& event) { updateHealthBar(event.health); });
	entityManager.getEvents().subscribe<DisplayCoins>([this](const auto& event) { updateCoinDisplay(event.inventory); });
	entityManager.getEvents().subscribe<PickedUpItem>([this](const auto& event) { updateItemsDisplay(event.item); });

	entityManager.getEvents().subscribe<DestroyEntity>([this](const auto& event) 
	{ 
		callbacks.push_back([this, event]
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
		callbacks.push_back([this, &stateData]
		{
			auto& entity = entityManager.createEntity("Resources/Files/Player.txt", stateData.games.front().getSpawnPoint());
			
			camera = stateData.window.getDefaultView();
			stateData.window.setView(camera);
		});
	});

	entityManager.getEvents().subscribe<Teleported>([this, &stateData](const auto& event)
	{
		callbacks.push_back([this, event]
		{ 
			changeLevel(event.level);
		});
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

	stateData.window.setView(camera);

	changeLevel(stateData.games.front().getCurrentLevel());
}

void GameState::handleEvent(const sf::Event& event)
{
	if (this->stateData.inputHandler.isActive("Escape"))
	{
		this->saveData("Resources/Files/SavedGames.txt");
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
	
	this->entityManager.update(deltaTime);
	this->coinDisplay.update(deltaTime);
	this->itemsDisplay.update(deltaTime);

	for (auto& callback : this->callbacks)
	{
		callback();
	}

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
}

void GameState::updateCamera(Entity entity)
{
	const auto& playerPosition = entity.get_component<PositionComponent>().getPosition();
	
	if (playerPosition.x > this->camera.getSize().x / 2.f && playerPosition.x < this->map.getBounds().width - this->camera.getSize().x / 2.f)
	{
		this->camera.setCenter(playerPosition.x, this->camera.getCenter().y);
		this->stateData.window.setView(this->camera);
	}
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
	this->entityManager.destroyEntities();

	this->map.load("Resources/Files/" + level + ".tmx");
	
	if (!this->stateData.games.front().getLevels().get<1>().find(level)->isLoaded)
	{
		this->entityManager.parseBlueprint("Resources/Files/BlueprintObjects-" + level + ".txt");
		this->stateData.games.front().getLevels().get<1>().modify(this->stateData.games.front().getLevels().get<1>().find(level), [](auto& iLevel) { iLevel.isLoaded = true; });
	}
	else
	{
		this->entityManager.parseEntities("Resources/Files/Entities-" + this->stateData.games.front().getGameName() + '-' + level + ".txt");
	}

	this->entityManager.parseBlueprint("Resources/Files/BlueprintEntities-" + level + ".txt");

	this->stateData.games.front().setCurrentLevel(level);

	this->camera = this->stateData.window.getDefaultView();
	this->stateData.soundManager.stopAllSounds();
}

void GameState::saveData(const std::string& pathFile)
{
	this->entityManager.saveEntities("Resources/Files/Entities-" + this->stateData.games.front().getGameName() + '-' + this->stateData.games.front().getCurrentLevel() + ".txt");
	
	std::ofstream outFile(pathFile);

	for (const auto& game : this->stateData.games)
	{
		outFile << game << '\n';
	}
}