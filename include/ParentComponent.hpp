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
	ParentComponent();
	ParentComponent(std::int32_t childID);

	std::int32_t getChildID() const;

	void setChildID(std::int32_t childID);

private:
	std::int32_t childID;
};

std::ostream& operator<<(std::ostream& os, const ParentComponent& component);