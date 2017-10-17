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

#include <set>


class RenderSystem : public System, public sf::Drawable
{
	using Renderables = brigand::list<SpriteComponent, TextComponent, DialogComponent, ParticleComponent>;

public:
	RenderSystem(Entities& entities, Events& events);

	virtual void update(float deltaTime) override;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setParentTransforms(Entity entity, ChildComponent& child);

	void addNewTransform(ChildComponent& child, ParentComponent& parent);

	std::int32_t getNewTransformationID() const;

	bool isInsideView(const sf::View& view, const sf::Vector2f& position, const sf::FloatRect& globalBounds) const;

	std::set<std::int32_t> transformationIDs;
};