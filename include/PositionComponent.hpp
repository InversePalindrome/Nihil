/*
Copyright (c) 2017 InversePalindrome
Nihil - PositionComponent.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/System/Vector2.hpp>


class PositionComponent
{
public:
	PositionComponent();
	PositionComponent(const sf::Vector2f& position);

	sf::Vector2f getPosition() const;

	void setPosition(const sf::Vector2f& position);

	void move(const sf::Vector2f& displacement);

private:
	sf::Vector2f position;
};