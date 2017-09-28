/*
Copyright (c) 2017 InversePalindrome
Nihil - MathUtility.hpp
InversePalindrome.com
*/


#pragma once

#include "UnitConverter.hpp"

#include <SFML/System/Vector2.hpp>

#include <cmath>


namespace MathUtils
{
	template<typename T>
	auto distance(const T& pointA, const T& pointB)
	{
		return  std::sqrt(std::pow(pointA.x - pointB.x, 2) + std::pow(pointA.y - pointB.y, 2));
	}

	template<typename T>
	auto calculateRequiredVelocity(T gravity, T distance, T angle, T launchHeight)
	{
		return std::sqrt((std::pow(distance, 2) * gravity) / (2 * std::cos(std::pow(UnitConverter::degreesToRadians(angle), 2)) * 
			(launchHeight + distance * std::tan(UnitConverter::degreesToRadians(angle)))));
	}
}