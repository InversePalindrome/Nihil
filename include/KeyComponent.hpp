/*
Copyright (c) 2017 InversePalindrome
Nihil - KeyComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"


class KeyComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const KeyComponent& component);

public:
	KeyComponent(std::size_t keyID);

	std::size_t getKeyID() const;

	void setKeyID(std::size_t keyID);

private:
	std::size_t keyID;
};

std::ostream& operator<<(std::ostream& os, const KeyComponent& component);