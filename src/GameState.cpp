/*
Copyright (c) 2017 InversePalindrome
Nihil - GameState.cpp
InversePalindrome.com
*/


#include "GameState.hpp"
#include "ControlSystem.hpp"
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
	powerUpDisplay(stateData.resourceManager),
	achievementDisplay(stateData.resourceManager)
{
	entityManager.copyBlueprint("Player.txt", stateData.games.front().getGameName() + "-Player.txt");

	world.SetContactListener(&collisionHandler);
	
	healthBar.setPosition(100.f, 70.f);

	coinDisplay.setPosition(1600.f, 60.f);
	coinDisplay.setNumberOfCoins(stateData.games.front().getItems()[Item::Coin]);

	itemsDisplay.setPosition(500.f, 110.f);

	powerUpDisplay.setPosition(500.f, 50.f);
	achievementDisplay.setPosition(740.f, 60.f);
	
	entityManager.getEvents().subscribe<DestroyBody>([this](const auto& event) { destroyBody(event.physics); });
	entityManager.getEvents().subscribe<DisplayHealthBar>([this](const auto& event) { updateHealthBar(event.health); });
	entityManager.getEvents().subscribe<DisplayCoins>([this](const auto& event) { updateCoinDisplay(); });
	entityManager.getEvents().subscribe<PickedUpItem>([this](const auto& event) { updateItemsDisplay(event.item); });
	entityManager.getEvents().subscribe<DisplayPowerUp>([this](const auto& event) { powerUpDisplay.addPowerUp(event.item); });
	entityManager.getEvents().subscribe<DisplayConversation>([this](const auto& event) { displayConversation(event.entity, event.visibilityStatus); });

	entityManager.getEvents().subscribe<UpdateConversation>([this](const auto& event) { updateConversationDisplay(event.entity); });
	entityManager.getEvents().subscribe<UpdateAchievement>([this](const auto& event) { updateAchievements(event.achievement); });
	
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
			auto player = entityManager.createEntity(stateData.games.front().getGameName() + "-Player.txt", stateData.games.front().getCurrentSpawnPoint());

			stateData.games.front().setPlayer(player);
			
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

		 saveData("SavedGames.txt");
	});

	entityManager.getEvents().subscribe<entityplus::component_added<Entity, ControllableComponent>>([this](auto& event)
	{
		entityManager.getSystem<ControlSystem>()->addControl(event.entity);
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
	if (this->stateData.inputHandler.isActive(Action::Escape))
	{
		this->saveData("SavedGames.txt");
		this->showWidgets(false);

		this->stateMachine.pushState(StateID::Pause);
	}
	else if (this->stateData.inputHandler.isActive(Action::Inventory))
	{
		this->itemsDisplay.setVisibility(!itemsDisplay.getVisibility());
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
	this->achievementDisplay.update();

	this->callbacks.update();
	this->callbacks.clearCallbacks();
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
	this->stateData.window.draw(this->achievementDisplay);
}

void GameState::showWidgets(bool showStatus)
{
	this->healthBar.setVisibilityStatus(showStatus);
	this->coinDisplay.setVisibilityStatus(showStatus);

	this->updateCoinDisplay();
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

void GameState::updateAchievements(Achievement achievement)
{
	auto& achievements = this->stateData.games.front().getAchievements();

	++achievements[achievement].first;

	if (achievements[achievement].first == achievements[achievement].second)
	{
		this->achievementDisplay.displayAchievement(achievement);

		this->entityManager.getEvents().broadcast(EmitSound{ SoundBuffersID::Achievement });
	}
}

void GameState::updateHealthBar(const HealthComponent& healthComponent)
{
	this->healthBar.setHitpointsDisplay(healthComponent.getHitpoints());
}

void GameState::updateCoinDisplay()
{
	this->coinDisplay.setNumberOfCoins(this->stateData.games.front().getItems()[Item::Coin]);
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

void GameState::updateConversationDisplay(Entity entity)
{
	if (entity.has_component<DialogComponent>())
	{
		auto& dialog = entity.get_component<DialogComponent>();

		if (dialog.isVisible())
		{
			dialog.nextDialogue();

			if (!dialog.hasDialogueFinished())
			{
				this->callbacks.addCallbackTimer([this, entity]() mutable
				{
					if (entity.get_status() == entityplus::entity_status::OK)
					{
						entity.sync();

						this->entityManager.getEvents().broadcast(UpdateConversation{ entity });
					}
				}, dialog.getDialogueTime());
			}
			else
			{
				this->callbacks.addCallbackTimer([entity]() mutable
				{
					if (entity.get_status() == entityplus::entity_status::OK)
					{
						entity.sync();

						entity.get_component<DialogComponent>().setVisibilityStatus(false);
					}
				}, dialog.getDialogueTime());
			}
		}
	}
}

void GameState::displayConversation(Entity entity, bool visibilityStatus)
{
	if (entity.has_component<DialogComponent>())
	{
		entity.get_component<DialogComponent>().setVisibilityStatus(visibilityStatus);
	}
}

void GameState::changeLevel(const std::string& level)
{
	auto& game = this->stateData.games.front();

	game.setCurrentLevel(level);

	this->entityManager.destroyEntities();
	
	this->map.load(level + ".tmx");

	if (!this->stateData.games.front().getLevels().get<1>().find(level)->isLoaded)
	{
		game.getLevels().get<1>().modify(game.getLevels().get<1>().find(level), [](auto& iLevel) { iLevel.isLoaded = true; });
		this->entityManager.parseBlueprint("Objects-" + level + ".txt");

		this->entityManager.createEntity(game.getGameName() + "-Player.txt", game.getCurrentSpawnPoint());
	}
	else
	{
		this->entityManager.parseEntities(game.getGameName() + '-' + level + ".txt");
	}

	this->entityManager.parseBlueprint("Entities-" + level + ".txt");
	this->stateData.soundManager.stopAllSounds();
	this->powerUpDisplay.clearPowerUps();

	this->entityManager.getEntities().for_each<ControllableComponent, PositionComponent, PhysicsComponent>([this, &game](auto entity, auto& controllable, auto& position, auto& physics)
	{
		const auto& spawnPoint = game.getCurrentSpawnPoint();

		position.setDialoguePosition({ spawnPoint.x, spawnPoint.y });
		physics.setDialoguePosition({ UnitConverter::pixelsToMeters(spawnPoint.x), UnitConverter::pixelsToMeters(-spawnPoint.y) });

		game.setPlayer(entity);
		
		this->moveCamera(spawnPoint);
	});

	this->world.SetGravity(game.getCurrentGravity());
}

void GameState::changeEntityPosition(Entity entity, const sf::Vector2f& position)
{
	if (entity.has_component<PositionComponent>())
	{
		entity.get_component<PositionComponent>().setDialoguePosition(position);
	}
	if (entity.has_component<PhysicsComponent>())
	{
		this->callbacks.addCallback([entity, position]() mutable
		{
			entity.get_component<PhysicsComponent>().setDialoguePosition({ UnitConverter::pixelsToMeters(position.x), UnitConverter::pixelsToMeters(-position.y) });
		});
	}
}

void GameState::destroyBody(PhysicsComponent& physics)
{
	this->world.DestroyBody(physics.getBody());
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