/*
Copyright (c) 2017 InversePalindrome
Nihil - EntityUtility.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "UnitConverter.hpp"


namespace Utility
{
	template<typename T>
	void setPosition(Entity entity, const T& position)
	{
		if (entity.has_component<PositionComponent>())
		{
			entity.get_component<PositionComponent>().setPosition({ position.x, position.y });
		}
		if (entity.has_component<PhysicsComponent>())
		{
			entity.get_component<PhysicsComponent>().setPosition({ UnitConverter::pixelsToMeters(position.x), UnitConverter::pixelsToMeters(-position.y) });
		}
		if (entity.has_component<SpriteComponent>())
		{
			entity.get_component<SpriteComponent>().setPosition({ position.x, position.y });
		}
	}

	template<typename T>
	void setAngle(Entity entity, T angle)
	{
		if (entity.has_component<PhysicsComponent>())
		{
			entity.get_component<PhysicsComponent>().setAngle(angle);
		}
		if (entity.has_component<SpriteComponent>())
		{
			entity.get_component<SpriteComponent>().setRotation(angle);
		}
	}
}