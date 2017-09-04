/*
Copyright (c) 2017 InversePalindrome
Nihil - ParentComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <memory>
#include <cstddef>


class ParentComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const ParentComponent& component);

public:
	ParentComponent(std::size_t ID);

	std::size_t getID() const;

	void setID(std::size_t ID);

private:
	std::size_t ID;
};

std::ostream& operator<<(std::ostream& os, const ParentComponent& component);