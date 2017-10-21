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
	Component("PositionA"),
	position(xPosition, yPosition)
{
}

std::ostream& operator<<(std::ostream& os, const PositionComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.position.x << ' ' << component.position.y;

	return os;
}

sf::Vector2f PositionComponent::getPosition() const
{
	return this->position;
}

void PositionComponent::setDialoguePosition(const sf::Vector2f& position)
{
	this->position = position;
}

void PositionComponent::move(const sf::Vector2f& displacement)
{
	this->position += displacement;
}