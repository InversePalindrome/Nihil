/*
Copyright (c) 2017 InversePalindrome
Nihil - Renderable.cpp
InversePalindrome.com
*/


#include "Renderable.hpp"


Renderable::Renderable() :
	visibilityStatus(true)
{
}

sf::Vector2f Renderable::getOffset() const
{
	return this->offset;
}

bool Renderable::isVisible() const
{
	return this->visibilityStatus;
}

void Renderable::setOffset(const sf::Vector2f& offset)
{
	this->offset = offset;
}

void Renderable::setVisibilityStatus(bool visibilityStatus)
{
	this->visibilityStatus = visibilityStatus;
}