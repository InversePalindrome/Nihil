/*
Copyright (c) 2017 InversePalindrome
Nihil - ControllableComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"


class ControllableComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const ControllableComponent& component);

public:
	ControllableComponent();
};

std::ostream& operator<<(std::ostream& os, const ControllableComponent& component);
