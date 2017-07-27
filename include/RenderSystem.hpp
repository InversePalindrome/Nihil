/*
Copyright (c) 2017 InversePalindrome
Nihil - RenderSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class RenderSystem : public System
{
public:
	RenderSystem(Entities& entities, Events& events);

	virtual void update(float deltaTime) override;
	void draw(sf::RenderTarget& target);
};