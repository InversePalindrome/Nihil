/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentParser.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "ResourceManager.hpp"

#include <box2d/Dynamics/b2World.h>

#include <string>
#include <sstream>
#include <functional>
#include <unordered_map>


class ComponentParser
{
public:
    ComponentParser(Entities& entities, ResourceManager& resourceManager, b2World& world);

    Entity parseEntity(std::int32_t entityType, const std::string& fileName);

    void parseBlueprint(const std::string& fileName);
    void parseEntities(const std::string& fileName);

    void copyBlueprint(const std::string& fileName, const std::string& copiedFileName);

private:
    Entities& entities;
    b2World& world;

    std::int32_t currentEntityID;
    std::unordered_map<std::string, std::function<void(Entity&, const std::string&)>> componentParsers;

    Entity createEntity();
    Entity parseComponents(std::int32_t entityID, const std::string& fileName);

    void setComponentsID(Entity entity, std::int32_t entityID);

    template <typename T>
    std::tuple<T> parse(std::istream& iStream);

    template <typename T, typename Arg, typename... Args>
    std::tuple<T, Arg, Args...> parse(std::istream& iStream);

    template <typename... Args>
    std::tuple<Args...> parse(const std::string& str);
};

template <typename T>
std::tuple<T> ComponentParser::parse(std::istream& iStream)
{
    T value;

    iStream >> value;

    return std::tuple<T>(std::move(value));

}

template <typename T, typename Arg, typename... Args>
std::tuple<T, Arg, Args...> ComponentParser::parse(std::istream& iStream)
{
    T value;

    iStream >> value;

    return std::tuple_cat(std::tuple<T>(std::move(value)), parse<Arg, Args...>(iStream));
}

template <typename... Args>
std::tuple<Args...> ComponentParser::parse(const std::string& line)
{
    std::istringstream iStream(line);

    return parse<Args...>(iStream);
}