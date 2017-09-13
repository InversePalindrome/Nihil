/*
Copyright (c) 2017 InversePalindrome
Nihil - AISystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"


class AISystem : public System
{
public:
	AISystem(Entities& entities, Events& events);

	virtual void update(float deltaTime) override;
};
