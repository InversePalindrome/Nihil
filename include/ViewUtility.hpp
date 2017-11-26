/*
Copyright (c) 2017 InversePalindrome
Nihil - ViewUtility.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>


namespace Utility
{
	bool isInsideView(const sf::View& view, const sf::Vector2f& position, const sf::FloatRect& globalBounds);
}