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
	underWaterDisplay(stateData.resourceManager),
	achievementDisplay(stateData.resourceManager)
{
	entityManager.copyBlueprint("Player.txt", stateData.games.front().getGameName() + "-Player.txt");

	world.SetContactListener(&collisionHandler);
	
	healthBar.setPosition(100.f, 70.f);

	coinDisplay.setPosition(1600.f, 60.f);
	coinDisplay.setNumberOfCoins(stateData.games.front().getItems()[Item::Coin]);

	underWaterDisplay.setPosition(600.f, 45.f);
	underWaterDisplay.setNumberOfBubbles(0u);

	powerUpDisplay.setPosition(1050.f, 50.f);
	achievementDisplay.setPosition(740.f, 60.f);
	itemsDisplay.setPosition(500.f, 160.f);

	entityManager.getEvents().subscribe<CrossedCheckpoint>([this](const auto& event) { setCheckpoint(event.position);  });
	entityManager.getEvents().subscribe<SetPosition>([this](const auto& event) { setPosition(event.entity, event.position); });

	entityManager.getEvents().subscribe<DestroyBody>([this](const auto& event) { destroyBody(event.physics); });
	entityManager.getEvents().subscribe<DestroyEntity>([this](const auto& event)
	{
		callbacks.addCallback([this, event]()
		{
			destroyEntity(event.entity);
		});
	});

	entityManager.getEvents().subscribe<DisplayHealthBar>([this](const auto& event) { updateHealthBar(event.health); });
	entityManager.getEvents().subscribe<DisplayCoins>([this](const auto& event) { updateCoinDisplay(); });
	entityManager.getEvents().subscribe<PickedUpItem>([this](const auto& event) { updateItemsDisplay(event.item); });
	entityManager.getEvents().subscribe<DisplayPowerUp>([this](const auto& event) { powerUpDisplay.addPowerUp(event.item); });
	entityManager.getEvents().subscribe<DisplayConversation>([this](const auto& event) { displayConversation(event.entity, event.visibilityStatus); });

	entityManager.getEvents().subscribe<UpdateConversation>([this](const auto& event) { updateConversationDisplay(event.entity); });
	entityManager.getEvents().subscribe<UpdateAchievement>([this](const auto& event) { updateAchievements(event.achievement); });

	entityManager.getEvents().subscribe<AddUnderWaterTimer>([this](const auto& event) { manageUnderWaterTimer(event.entity); });
    entityManager.getEvents().subscribe<RemoveUnderWaterTimer>([this](const auto& event) { removeUnderWaterTimer(event.entity); });
	
	entityManager.getEvents().subscribe<HidePowerUp>([this](const auto& event) { powerUpDisplay.removePowerUp(event.item); });

	entityManager.getEvents().subscribe<CreateEntity>([this](const auto& event) 
	{ 
		callbacks.addCallback([this, event]()
		{
			entityManager.createEntity(event.fileName, event.position);
		});
	});

	entityManager.getEvents().subscribe<ChangeLevel>([this, &stateData](const auto& event)
	{
		callbacks.addCallback([this, event]
		{ 
			changeLevel(event.level, event.position);
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

	changeLevel(stateData.games.front().getCurrentLevel(), stateData.games.front().getSpawnpoint());
}

void GameState::handleEvent(const sf::Event& event)
{
	if (this->stateData.inputHandler.isActive(Action::Escape))
	{
		this->saveData("SavedGames.txt");
		this->showWidgets(false);

		this->stateData.soundManager.stopAllSounds();
		this->stateData.soundManager.stopAllMusic();

		this->entityManager.getEntities().for_each<AutomatedComponent>([](auto entity, auto& automated)
		{
			automated.stopCurrentTask();
		});

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

	this->entityManager.getEntities().for_each<TimerComponent>([](auto entity, auto& timer)
	{
		timer.update();
	});

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
	this->stateData.window.draw(this->underWaterDisplay);
	this->stateData.window.draw(this->achievementDisplay);
}

void GameState::showWidgets(bool showStatus)
{
	this->healthBar.setVisibilityStatus(showStatus);
	this->coinDisplay.setVisibilityStatus(showStatus);

	this->updateCoinDisplay();
}

void GameState::updateCamera()
{
	this->entityManager.getEntities().for_each<ControllableComponent, PositionComponent>([this](auto entity, const auto& controllable, const auto& position)
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
			else if (centerPosition.x <= this->camera.getSize().x / 2.f)
			{
				this->camera.setCenter(this->stateData.window.getDefaultView().getCenter());
			}
			else
			{
				this->camera.setCenter(this->map.getBounds().width - this->camera.getSize().x / 2.f, this->stateData.window.getDefaultView().getCenter().y);
			}
		}
		break;
		case DirectionType::Vertical:
		{
			if (centerPosition.y > this->camera.getSize().y / 2.f && centerPosition.y < this->map.getBounds().height - this->camera.getSize().y / 2.f)
			{
				this->camera.setCenter(this->camera.getCenter().x, centerPosition.y);
			}
			else if (centerPosition.y <= this->camera.getSize().y / 2.f)
			{
				this->camera.setCenter(this->stateData.window.getDefaultView().getCenter());
			}
			else
			{
				this->camera.setCenter(this->stateData.window.getDefaultView().getCenter().x, this->map.getBounds().height - this->camera.getSize().y / 2.f);
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
					if (entity.sync())
					{
						this->updateConversationDisplay(entity);
					}
				}, dialog.getDialogueTime());
			}
			else
			{
				this->callbacks.addCallbackTimer([entity]() mutable
				{
					if (entity.sync())
					{
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

void GameState::changeLevel(const std::string& level, const sf::Vector2f& spawnpoint)
{
	auto& game = this->stateData.games.front();

	game.setCurrentLevel(level);
	game.setSpawnpoint(spawnpoint);
	
	this->entityManager.destroyEntities();
	
	this->map.load(level + ".tmx");

	if (!this->stateData.games.front().getLevels().get<1>().find(level)->isLoaded)
	{
		game.getLevels().get<1>().modify(game.getLevels().get<1>().find(level), [](auto& iLevel) { iLevel.isLoaded = true; });

		this->entityManager.parseBlueprint("Objects-" + level + ".txt");
		this->entityManager.createEntity(game.getGameName() + "-Player.txt", game.getSpawnpoint());
	}
	else
	{
		this->entityManager.parseEntities(game.getGameName() + '-' + level + ".txt");
	}

	this->entityManager.parseBlueprint("Entities-" + level + ".txt");

	this->stateData.soundManager.stopAllSounds();
	this->stateData.soundManager.stopAllMusic();
	
	this->camera.setCenter(this->camera.getSize().x / 2.f, this->camera.getSize().y / 2.f);

	this->powerUpDisplay.clearPowerUps();

	this->entityManager.getEntities().for_each<ControllableComponent, PositionComponent, PhysicsComponent>(
		[this, spawnpoint, &game](auto player, const auto& control, const auto& position, const auto& physics)
	{
		this->setPosition(player, spawnpoint);
		
		game.setPlayer(player);
	});

	this->world.SetGravity(game.getCurrentGravity());
}

void GameState::setCheckpoint(const sf::Vector2f& position)
{
	this->stateData.games.front().setSpawnpoint(position);
}

void GameState::setPosition(Entity entity, const sf::Vector2f& position)
{
	if (entity.has_component<PositionComponent>())
	{
		entity.get_component<PositionComponent>().setPosition(position);
	}
	if (entity.has_component<PhysicsComponent>())
	{
		this->callbacks.addCallback([entity, position]() mutable
		{
			entity.sync();

			entity.get_component<PhysicsComponent>().setPosition({ UnitConverter::pixelsToMeters(position.x), UnitConverter::pixelsToMeters(-position.y) });
		});
	}
}

void GameState::destroyEntity(Entity entity)
{
	this->entityManager.destroyEntity(entity);

	if (entity.has_component<ControllableComponent>())
	{
		this->stateData.games.front().setPlayer(this->entityManager.createEntity(stateData.games.front().getGameName()
				+ "-Player.txt", stateData.games.front().getSpawnpoint()));

		this->powerUpDisplay.clearPowerUps();
	}
}

void GameState::destroyBody(PhysicsComponent& physics)
{
	this->world.DestroyBody(physics.getBody());
}

void GameState::manageUnderWaterTimer(Entity entity)
{
	if (entity.has_component<StateComponent>() && entity.get_component<StateComponent>().getState() != EntityState::Swimming)
	{
		const float timePerBubble = 1.5f;
		std::size_t numberOfBubbles = 5u;

		this->underWaterDisplay.setNumberOfBubbles(numberOfBubbles);

		this->addUnderWaterTimer(entity, numberOfBubbles, timePerBubble, true);
	}
}

void GameState::addUnderWaterTimer(Entity entity, std::size_t numberOfBubbles, float timePerBubble, bool forceCallback)
{
	if (forceCallback || (entity.has_component<StateComponent>() && entity.get_component<StateComponent>().getState() == EntityState::Swimming))
	{
		this->callbacks.addCallbackTimer([this, entity, numberOfBubbles, timePerBubble]() mutable
		{
			if (entity.sync())
			{
				--numberOfBubbles;

				this->underWaterDisplay.setNumberOfBubbles(numberOfBubbles);

				if (!numberOfBubbles && entity.has_component<StateComponent>() && entity.get_component<StateComponent>().getState() == EntityState::Swimming)
				{
					this->destroyEntity(entity);
				}
				else
				{
					this->addUnderWaterTimer(entity, numberOfBubbles, timePerBubble, false);
				}
			}
		}, timePerBubble);
	}
}

void GameState::removeUnderWaterTimer(Entity entity)
{
	if (entity.has_component<PhysicsComponent>() && !entity.get_component<PhysicsComponent>().isColliding(ObjectType::Head, ObjectType::Liquid))
	{
		this->callbacks.disconnectCallbackTimers();
		this->underWaterDisplay.setNumberOfBubbles(0u);
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