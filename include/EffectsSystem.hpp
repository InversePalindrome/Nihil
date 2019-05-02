/*
Copyright (c) 2017 InversePalindrome
Nihil - EffectsSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"


class EffectsSystem : public System
{
public:
    EffectsSystem(Entities& entities, Events& events);

    virtual void update(float deltaTime) override;
};