/*
Copyright (c) 2017 InversePalindrome
Nihil - PowerUpDisplay.hpp
InversePalindrome.com
*/


#pragma once

#include "Item.hpp"
#include "Animation.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <unordered_map>


struct PowerUpGraphics;
struct PowerUpData;

class PowerUpDisplay : public sf::Drawable, public sf::Transformable
{
public:
    PowerUpDisplay(ResourceManager& resourceManager);

    void update(float deltaTime);

    void addPowerUp(Item powerUp);
    void removePowerUp(Item powerUp);
    void clearPowerUps();

private:
    std::unordered_map<Item, PowerUpGraphics> powerUps;
    std::unordered_map<Item, PowerUpData> powerUpData;

    ResourceManager& resourceManager;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

struct PowerUpGraphics : public sf::Drawable, public sf::Transformable
{
    PowerUpGraphics(ResourceManager& resourceManager, Item item, const std::unordered_map<Item, PowerUpData>& data);

    sf::Sprite sprite;
    Animator animator;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

struct PowerUpData
{
    std::string animationFile;
    sf::IntRect textureRect;
    float scale;
};