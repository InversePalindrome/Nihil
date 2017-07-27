/*
Copyright (c) 2017 InversePalindrome
Nihil - PositionComponent.cpp
InversePalindrome.com
*/


#include "PositionComponent.hpp"


PositionComponent::PositionComponent(const sf::Vector2f& position) :
	position(position)
{
}

sf::Vector2f PositionComponent::getPosition() const
{
	return this->position;
}

void PositionComponent::setPosition(const sf::Vector2f& position)
{
	this->position = position;
}

void PositionComponent::move(const sf::Vector2f& displacement)
{
	this->position += displacement;
}