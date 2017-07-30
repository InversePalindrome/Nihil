/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimatorSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"


class AnimatorSystem : public System
{
public:
	AnimatorSystem(Entities& entities, Events& events);

	virtual void update(float deltaTime) override;

private:
	void changeAnimationState(Entity entity, EntityState state);
	void changeAnimationDirection(Entity entity, Direction direction);
};