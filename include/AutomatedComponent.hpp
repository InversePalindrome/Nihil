/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatedComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Direction.hpp"

#include <Box2D/Common/b2Math.h>

#include <vector>
#include <string>


class AutomatedComponent : public Component
{
    friend std::ostream& operator<<(std::ostream& os, const AutomatedComponent& component);
    using Task = std::pair<Direction, b2Vec2>;

public:
    AutomatedComponent();

    void loadTasks(const std::string& fileName);

    void pushNextTask();

    Task getCurrentTask() const;

    bool hasTasks() const;

private:
    std::vector<Task> tasks;

    std::size_t taskIndex;
};

std::ostream& operator<<(std::ostream& os, const AutomatedComponent& component);