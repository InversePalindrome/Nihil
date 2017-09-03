/*
Copyright (c) 2017 InversePalindrome
Nihil - PositionComponent.cpp
InversePalindrome.com
*/


#include "PositionComponent.hpp"


PositionComponent::PositionComponent() :
	PositionComponent(0.f, 0.f)
{
}

PositionComponent::PositionComponent(float xPosition, float yPosition) :
	Component(ComponentID::Position),
	position(xPosition, yPosition)
{
}

std::ostream& PositionComponent::operator<<(std::ostream& os)
{
	os << this->position.x << ' ' << this->position.y;

	return os;
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