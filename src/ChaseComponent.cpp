/*
Copyright (c) 2017 InversePalindrome
Nihil - ChaseComponent.cpp
InversePalindrome.com
*/


#include "ChaseComponent.hpp"


ChaseComponent::ChaseComponent(float visionRange) :
    Component("Chase"),
    visionRange(visionRange)
{
}

std::ostream& operator<<(std::ostream& os, const ChaseComponent& component)
{
    os << component.getEntityID() << ' ' << component.getName() << ' ' << component.visionRange;

    return os;
}

float ChaseComponent::getVisionRange() const
{
    return this->visionRange;
}

void ChaseComponent::setVisionRange(float visionRange)
{
    this->visionRange = visionRange;
}