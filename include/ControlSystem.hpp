/*
Copyright (c) 2017 InversePalindrome
Nihil - ControlSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "InputHandler.hpp"


class ControlSystem : public System
{
public:
	ControlSystem(Entities& entities, Events& events, InputHandler& inputHandler);

	virtual void update(float deltaTime) override;

private:
	InputHandler& inputHandler;

	float timeSinceJump;
	static constexpr float jumpInterval = 0.6f;

	void reactToInput(Entity entity, float deltaTime);
};