/*
Copyright (c) 2017 InversePalindrome
Nihil - ViewUtility.cpp
InversePalindrome.com
*/


#include "ViewUtility.hpp"


bool Utility::isInsideView(const sf::View& view, const sf::Vector2f& position, const sf::FloatRect& globalBounds)
{
	const auto& left = view.getCenter().x - view.getSize().x / 2.f;
	const auto& right = view.getCenter().x + view.getSize().x / 2.f;
	const auto& top = view.getCenter().y - view.getSize().y / 2.f;
	const auto& bottom = view.getCenter().y + view.getSize().y / 2.f;

	return position.x + globalBounds.width / 2.f > left && position.x - globalBounds.width / 2.f < right
		&& position.y + globalBounds.height / 2.f > top && position.y - globalBounds.height / 2.f < bottom;
}