/*
Copyright (c) 2017 InversePalindrome
Nihil - RenderSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"

#include <brigand/sequences/list.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class RenderSystem : public System, public sf::Drawable
{
	using Renderables = brigand::list<SpriteComponent, TextComponent, DialogComponent, ParticleComponent>;

public:
	RenderSystem(Entities& entities, Events& events);

	virtual void update(float deltaTime) override;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setParentTransforms(Entity childEntity, Entity parentEntity);
};