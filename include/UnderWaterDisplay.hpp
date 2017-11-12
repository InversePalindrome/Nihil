/*
Copyright (c) 2017 InversePalindrome
Nihil - UnderWaterDisplay.hpp
InversePalindrome.com
*/


#pragma once

#include "Renderable.hpp"


class UnderWaterDisplay : public Renderable
{
public:
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};