/*
Copyright (c) 2017 InversePalindrome
Nihil - ControlSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "InputHandler.hpp"
#include "Callbacks.hpp"


class ControlSystem : public System
{
public:
	ControlSystem(Entities& entities, Events& events, InputHandler& inputHandler);

	void addControl(Entity entity);

	virtual void update(float deltaTime) override;

private:
	Callbacks callbacks;
	InputHandler& inputHandler;

	void setMidAirStatus(Entity entity, bool midAirStatus);
};