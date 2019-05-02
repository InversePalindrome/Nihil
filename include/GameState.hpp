/*
Copyright (c) 2017 InversePalindrome
Nihil - GameState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"
#include "Map.hpp"
#include "CollisionData.hpp"
#include "CollisionHandler.hpp"
#include "CollisionFilter.hpp"
#include "Callbacks.hpp"
#include "EntityManager.hpp"
#include "HealthBar.hpp"
#include "CoinDisplay.hpp"
#include "ItemsDisplay.hpp"
#include "PowerUpDisplay.hpp"
#include "AchievementDisplay.hpp"
#include "UnderWaterDisplay.hpp"
#include "Pathway.hpp"

#include <Box2D/Dynamics/b2World.h>

#include <SFML/Graphics/View.hpp>

#include <memory>


class GameState : public State
{
public:
    GameState(StateMachine& stateMachine, StateData& stateData);

    virtual void handleEvent(const sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void draw() override;

    virtual void showWidgets(bool showStatus) override;

private:
    b2World world;
    EntityManager entityManager;
    CollisionsData collisionsData;

    Map map;
    Entity player;
    sf::View camera;
    Callbacks callbacks;
    Pathways pathways;

    CollisionHandler collisionHandler;
    CollisionFilter collisionFilter;

    HealthBar healthBar;
    CoinDisplay coinDisplay;
    ItemsDisplay itemsDisplay;
    PowerUpDisplay powerUpDisplay;
    UnderWaterDisplay underWaterDisplay;
    AchievementDisplay achievementDisplay;

    void updateCamera();
    void updateAchievements(Achievement achievement);
    void updateHealthBar(const HealthComponent& health);
    void updateCoinDisplay();
    void updateItemsDisplay(Entity item);
    void updateConversationDisplay(Entity entity);

    void displayConversation(Entity entity, bool visibilityStatus);

    void changeLevel(const std::string& level, const sf::Vector2f& spawnpoint);

    void setCheckpoint(const sf::Vector2f& position);

    void destroyEntity(Entity entity);
    void destroyBody(PhysicsComponent& physics);

    void manageUnderWaterTimer(Entity entity);
    void addUnderWaterTimer(Entity entity, std::size_t numberOfBubbles, float timePerBubble);
    void removeUnderWaterTimer(Entity entity);

    void saveData(const std::string& fileName);
};