/*
Copyright (c) 2017 InversePalindrome
Nihil - HealthComponent.hpp
InversePalindrome.com
*/


#pragma once

#include <cstddef>


class HealthComponent
{
public:
	HealthComponent(std::size_t hitpoints);

	std::size_t getHitpoints() const;

	void setHitpoints(std::size_t hitpoints);

private:
	std::size_t hitpoints;
};