/*
Copyright (c) 2017 InversePalindrome
Nihil - HealthBar.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>


class HealthBar : public sf::Drawable, public sf::Transformable
{
public:
	HealthBar(ResourceManager& resourceManager);

	void setHitpointsDisplay(std::size_t hitpoints);

private:
	sf::Sprite heart;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};