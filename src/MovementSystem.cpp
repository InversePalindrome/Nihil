/*
Copyright (c) 2017 InversePalindrome
Nihil - MovementSystem.cpp
InversePalindrome.com
*/


#include "MovementSystem.hpp"
#include "PositionComponent.hpp"



MovementSystem::MovementSystem(Entities& entities, Events& events) :
	System(entities, events)
{
}

void MovementSystem::update(float deltaTime)
{
	this->entities.for_each<PositionComponent>
		([deltaTime](auto entity, auto& position, auto& velocity)
	{
		
	});
}