/*
Copyright (c) 2017 InversePalindrome
Nihil - Renderable.hpp
InversePalindrome.com
*/


#pragma once


#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>


class Renderable : public sf::Transformable, public sf::Drawable
{
public:
    Renderable();

    sf::Vector2f getOffset() const;
    bool isVisible() const;

    void setOffset(const sf::Vector2f& offset);

    void setVisibilityStatus(bool visibilityStatus);

private:
    sf::Vector2f offset;
    bool visibilityStatus;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};