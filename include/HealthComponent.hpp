/*
Copyright (c) 2017 InversePalindrome
Nihil - HealthComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <cstdint>


class HealthComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const HealthComponent& component);

public:
	HealthComponent(std::int32_t hitpoints);

	std::int32_t getHitpoints() const;

	void setHitpoints(std::int32_t hitpoints);

private:
	std::int32_t hitpoints;
};

std::ostream& operator<<(std::ostream& os, const HealthComponent& component);