/*
Copyright (c) 2017 InversePalindrome
Nihil - ChaseComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"


class ChaseComponent : public Component
{
    friend std::ostream& operator<<(std::ostream& os, const ChaseComponent& component);

public:
    ChaseComponent(float visionRange);

    float getVisionRange() const;

    void setVisionRange(float visionRange);

private:
    float visionRange;
};

