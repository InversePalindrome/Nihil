/*
Copyright (c) 2017 InversePalindrome
Nihil - HealthComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <cstddef>


class HealthComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const HealthComponent& component);

public:
	HealthComponent(std::size_t hitpoints);

	std::size_t getHitpoints() const;

	void setHitpoints(std::size_t hitpoints);

private:
	std::size_t hitpoints;
};

std::ostream& operator<<(std::ostream& os, const HealthComponent& component);