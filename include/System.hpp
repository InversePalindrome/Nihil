/*
Copyright (c) 2017 InversePalindrome
Nihil - System.hpp
InversePalindrome.com
*/

#pragma once

#include "ECS.hpp"


class System
{
public:
	System(Entities& entities, Events& events);

	virtual void update(float deltaTime) = 0;

protected:
	Entities& entities;
	Events& events;
};