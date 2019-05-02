/*
Copyright (c) 2017 InversePalindrome
Nihil - Game.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "Item.hpp"
#include "Achievement.hpp"
#include "Direction.hpp"

#include <Box2D/Common/b2Math.h>
#include <SFML/System/Vector2.hpp>

#include <cstddef>
#include <string>
#include <ostream>
#include <optional>
#include <unordered_map>


struct Level
{
    DirectionType directionType;
    b2Vec2 gravity;
    bool isLoaded;
    std::string musicFile;
};

class Game
{
    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    using Levels = std::unordered_map<std::string, Level>;

public:
    Game();
    Game(const std::string& data);

    std::optional<Entity> getPlayer();
    std::string getGameName() const;
    std::string getCurrentLevel() const;
    DirectionType getCurrenDirectionType() const;
    b2Vec2 getCurrentGravity() const;
    sf::Vector2f getSpawnpoint() const;
    std::string getLevelMusic() const;

    Levels& getLevels();
    Items& getItems();
    Achievements& getAchievements();

    void setPlayer(Entity player);
    void setGameName(const std::string& name);
    void setCurrentLevel(const std::string& currentLevel);
    void setSpawnpoint(const sf::Vector2f& spawnpoint);

private:
    Entity player;

    std::string gameName;
    std::string currentLevel;
    Items items;
    Achievements achievements;
    sf::Vector2f spawnpoint;

    Levels levels;

    void loadLevels();
    void loadAchievements();
};

std::ostream& operator<<(std::ostream& os, const Game& game);
