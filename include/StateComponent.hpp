/*
Copyright (c) 2017 InversePalindrome
Nihil - StateComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"


enum class EntityState { Idle, Walking, Jumping, Attacking, Dead };

class StateComponent : public Component
{
public:
	StateComponent();

	virtual std::ostream& operator<<(std::ostream& os) override;

	EntityState getState() const;

	void setState(EntityState state);

private:
	EntityState state;
};
