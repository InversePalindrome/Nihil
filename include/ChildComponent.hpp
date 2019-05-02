/*
Copyright (c) 2017 InversePalindrome
Nihil - ChildComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <cstddef>


class ChildComponent : public Component
{
    friend std::ostream& operator<<(std::ostream& os, const ChildComponent& component);

public:
    ChildComponent();
    ChildComponent(std::int32_t parentID);

    std::int32_t getParentID() const;
    const sf::Transform& getTransform() const;

    void setParentID(std::int32_t parentID);
    void setTransform(const sf::Transform& transform);

private:
    std::int32_t parentID;
    const sf::Transform* transform;
};

std::ostream& operator<<(std::ostream& os, const ChildComponent& component);