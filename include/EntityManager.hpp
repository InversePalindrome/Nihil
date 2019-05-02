/*
Copyright (c) 2017 InversePalindrome
Nihil - EntityManager.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "System.hpp"
#include "Pathway.hpp"
#include "InputHandler.hpp"
#include "CollisionData.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"
#include "ComponentParser.hpp"
#include "ComponentSerializer.hpp"

#include <Box2D/Dynamics/b2World.h>

#include <brigand/sequences/map.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <string>
#include <memory>
#include <typeinfo>
#include <unordered_map>


class EntityManager : public sf::Drawable
{
public:
    EntityManager(b2World& world, ResourceManager& resourceManager, SoundManager& soundManager,
        InputHandler& inputHandler, CollisionsData& collisionsData, Pathways& pathways);
    EntityManager(const EntityManager& entityManager) = delete;
    EntityManager& operator=(const EntityManager& entityManager) = delete;

    Entities& getEntities();
    Events& getEvents();
    ComponentSerializer& getComponentSerializer();

    template<typename T>
    T* getSystem();

    void update(float deltaTime);

    Entity createEntity(std::int32_t entityType, const std::string& fileName);
    Entity createEntity(std::int32_t entityType, const std::string& fileName, const sf::Vector2f& position);

    void parseEntities(const std::string& fileName);
    void parseBlueprint(const std::string& fileName);

    void loadEntityProperties();

    void copyBlueprint(const std::string& fileName, const std::string& copiedFileName);

    void destroyEntity(Entity entity);
    void destroyEntities();

    void saveEntities(const std::string& fileName);

private:
    Entities entityManager;
    Events eventManager;

    b2World& world;

    ComponentParser componentParser;
    ComponentSerializer componentSerializer;

    std::unordered_map<std::string, std::unique_ptr<System>> systems;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

template<typename T>
T* EntityManager::getSystem()
{
    return dynamic_cast<T*>(this->systems.at(typeid(T).name()).get());
}