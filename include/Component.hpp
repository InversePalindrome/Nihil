/*
(c) 2017 InversePalindrome
Nihil - Component.hpp
InversePalindrome.com
*/


#pragma once

#include <fstream>


enum class ComponentID { Position, State, Physics, AI, Health, Attack, Sprite, Animation, Sound, Particle, Parent, Child, Automated };

class Component
{
public:
	Component(ComponentID componentID);

	ComponentID getID() const;

	virtual std::ostream& operator<<(std::ostream& os) = 0;

private:
	ComponentID componentID;
};
