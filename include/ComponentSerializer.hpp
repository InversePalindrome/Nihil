/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentSerializer.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "Component.hpp"

#include <brigand/sequences/list.hpp>

#include <string>


class ComponentSerializer
{
public:
	ComponentSerializer(Entities& entities);

	void serialize(const std::string& pathFile);

private:
	Entities& entities;

	using ComponentList = brigand::list<PositionComponent, StateComponent, PhysicsComponent, AIComponent, 
		HealthComponent, AttackComponent, SpriteComponent, AnimationComponent, SoundComponent,
		ParticleComponent, ParentComponent, ChildComponent, AutomatedComponent>;
};