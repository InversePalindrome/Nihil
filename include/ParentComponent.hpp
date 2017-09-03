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
public:
	ParentComponent(std::size_t ID);

	virtual std::ostream& operator<<(std::ostream& os) override;

	std::size_t getID() const;

	void setID(std::size_t ID);

private:
	std::size_t ID;
};