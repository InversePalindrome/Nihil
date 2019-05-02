/*
Copyright (c) 2017 InversePalindrome
Nihil - StateSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "StateComponent.hpp"


class StateSystem : public System
{
public:
    StateSystem(Entities& entities, Events& events);

    virtual void update(float deltaTime) override;

private:
    void changeState(Entity entity, EntityState state);
};