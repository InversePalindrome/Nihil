/*
Copyright (c) 2017 InversePalindrome
Nihil - ChildComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <cstddef>


class ChildComponent : public Component
{
public:
	ChildComponent(std::size_t parentID);

	virtual std::ostream& operator<<(std::ostream& os) override;

	std::size_t getParentID() const;
	const sf::Transform& getTransform() const;

	void setParentID(std::size_t parentID);
	void setTransform(const sf::Transform& transform);

private:
	std::size_t parentID;
	const sf::Transform* transform;
};