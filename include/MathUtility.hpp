/*
Copyright (c) 2017 InversePalindrome
Nihil - MathUtility.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/System/Vector2.hpp>

#include <cmath>


namespace MathUtils
{
	float distance(const sf::Vector2f& pointA, const sf::Vector2f& pointB)
	{
		return std::sqrt(std::pow(pointA.x - pointB.x, 2) + std::pow(pointA.y - pointB.y, 2));
	}
}