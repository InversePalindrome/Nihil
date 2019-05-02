/*
Copyright (c) 2017 InversePalindrome
Nihil - DropComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Item.hpp"

#include <vector>
#include <optional>


class DropComponent : public Component
{
    friend std::ostream& operator<<(std::ostream& os, const DropComponent& component);

public:
    DropComponent(const std::string& fileName);

    std::optional<Item> getDrop() const;

private:
    std::vector<std::pair<Item, float>> drops;
};

std::ostream& operator<<(std::ostream& os, const DropComponent& component);