/*
Copyright (c) 2017 InversePalindrome
Nihil - HealthBar.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"
#include "Renderable.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class HealthBar : public Renderable
{
public:
    HealthBar(ResourceManager& resourceManager);

    void setHitpointsDisplay(std::size_t hitpoints);

private:
    sf::Sprite heart;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};