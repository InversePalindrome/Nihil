/*
Copyright (c) 2017 InversePalindrome
Nihil - AttackComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <cstddef>


class AttackComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const AttackComponent& component);

public:
	AttackComponent(std::size_t damagePoints);

	std::size_t getDamagePoints() const;

	void setDamagePoints(std::size_t damagePoints);

private:
	std::size_t damagePoints;
};

std::ostream& operator<<(std::ostream& os, const AttackComponent& component);