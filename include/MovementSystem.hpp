/*
Copyright (c) 2017 InversePalindrome
Nihil - MovementSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"


class MovementSystem : public System
{
public:
	MovementSystem(Entities& entities, Events& events);

	virtual void update(float deltaTime) override;
};