/*
Copyright (c) 2017 InversePalindrome
Nihil - StateComponent.hpp
InversePalindrome.com
*/


#pragma once


enum class EntityState { Idle, Walking, Jumping, Attacking };

class StateComponent
{
public:
	StateComponent();

	EntityState getState() const;

	void setState(EntityState state);

private:
	EntityState state;
};
