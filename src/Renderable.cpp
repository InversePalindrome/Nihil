/*
Copyright (c) 2017 InversePalindrome
Nihil - Renderable.cpp
InversePalindrome.com
*/


#include "Renderable.hpp"


sf::Vector2f Renderable::getOffset() const
{
	return this->offset;
}

void Renderable::setOffset(const sf::Vector2f& offset)
{
	this->offset = offset;
}