/*
Copyright (c) 2017 InversePalindrome
Nihil - RangeAttackComponent.cpp
InversePalindrome.com
*/


#include "RangeAttackComponent.hpp"


RangeAttackComponent::RangeAttackComponent(const std::string& projectileID, float reloadTime) :
    RangeAttackComponent(projectileID, reloadTime, 0.f)
{
}

RangeAttackComponent::RangeAttackComponent(const std::string& projectileID, float reloadTime, float attackRange) :
    Component("Range"),
    projectileID(projectileID),
    reloadTime(reloadTime),
    attackRange(attackRange)
{
}

std::ostream& operator<<(std::ostream& os, const RangeAttackComponent& component)
{
    os << component.getEntityID() << ' ' << component.getName() << ' ' << component.projectileID << ' ' << component.reloadTime << ' ' << component.attackRange;

    return os;
}

std::string RangeAttackComponent::getProjectileID() const
{
    return this->projectileID;
}

float RangeAttackComponent::getReloadTime() const
{
    return this->reloadTime;
}

float RangeAttackComponent::getAttackRange() const
{
    return this->attackRange;
}

void RangeAttackComponent::setProjectileID(const std::string& projectileID)
{
    this->projectileID = projectileID;
}

void RangeAttackComponent::setReloadTime(float reloadTime)
{
    this->reloadTime = reloadTime;
}

void RangeAttackComponent::setAttackRange(float attackRange)
{
    this->attackRange = attackRange;
}