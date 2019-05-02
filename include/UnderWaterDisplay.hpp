/*
Copyright (c) 2017 InversePalindrome
Nihil - UnderWaterDisplay.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"
#include "Renderable.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class UnderWaterDisplay : public Renderable
{
public:
    UnderWaterDisplay(ResourceManager& resourceManager);

    void setNumberOfBubbles(std::size_t numberOfBubbles);

private:
    sf::Sprite bubbleSprite;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};