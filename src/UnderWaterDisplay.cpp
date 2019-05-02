/*
Copyright (c) 2017 InversePalindrome
Nihil - UnderWaterDisplay.cpp
InversePalindrome.com
*/


#include "UnderWaterDisplay.hpp"


UnderWaterDisplay::UnderWaterDisplay(ResourceManager& resourceManager)
{
    auto& bubbleTexture = resourceManager.getTexture(TexturesID::Bubble);

    bubbleTexture.setRepeated(true);

    bubbleSprite.setTexture(bubbleTexture);
    bubbleSprite.setScale(0.25f, 0.25f);
}

void UnderWaterDisplay::setNumberOfBubbles(std::size_t numberOfBubbles)
{
    this->bubbleSprite.setTextureRect({ 0, 0, static_cast<int>(this->bubbleSprite.getTexture()->getSize().x
        * numberOfBubbles), static_cast<int>(this->bubbleSprite.getTexture()->getSize().y) });
}

void UnderWaterDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();

    target.draw(this->bubbleSprite, states);
}