/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentSerializer.cpp
InversePalindrome.com
*/


#include "ComponentSerializer.hpp"
#include "FilePaths.hpp"

#include <brigand/algorithms/for_each.hpp>

#include <boost/algorithm/string/trim.hpp>

#include <map>
#include <fstream>
#include <sstream>


ComponentSerializer::ComponentSerializer(Entities& entities) :
    entities(entities)
{
}

EntityProperties& ComponentSerializer::getProperties()
{
    return this->properties;
}

void ComponentSerializer::serialize(const std::string& fileName)
{
    std::multimap<std::int32_t, std::string> entities;

    brigand::for_each<ComponentList>([this, &entities](auto componentType)
        {
            using Type = decltype(componentType)::type;

            this->entities.for_each<Type>([&entities](auto entity, auto & component)
                {
                    std::stringstream stream;

                    std::int32_t entityID;
                    std::string componentData;

                    stream << component;

                    stream >> entityID;

                    if (entityID > 0)
                    {
                        std::getline(stream, componentData);

                        boost::trim_left(componentData);

                        entities.emplace(entityID, componentData);
                    }
                });
        });

    std::ofstream outFile(Path::games / fileName);

    for (auto entityItr = std::begin(entities); entityItr != std::end(entities);)
    {
        auto entityID = entityItr->first;

        outFile << "Entity " << entityID << '\n';

        do
        {
            outFile << entityItr->second << '\n';

            ++entityItr;
        } while (entityItr != std::end(entities) && entityID == entityItr->first);

        outFile << '\n';
    }
}

void ComponentSerializer::saveBlueprint(const std::string & fileName, const std::vector<std::tuple<std::int32_t, std::string, sf::Vector2f>> & entitiesData)
{
    std::ofstream outFile(Path::blueprints / fileName);

    for (const auto& [entityID, fileName, position] : entitiesData)
    {
        outFile << entityID << ' ' << fileName << ' ' << position.x << ' ' << position.y << '\n';
    }
}


void ComponentSerializer::setProperties(const EntityProperties & properties)
{
    this->properties = properties;
}