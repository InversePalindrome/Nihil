/*
Copyright (c) 2017 InversePalindrome
Nihil - ECS.hpp
InversePalindrome.com
*/


#pragma once

#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "StateComponent.hpp"
#include "PositionComponent.hpp"
#include "AIComponent.hpp"
#include "AnimationComponent.hpp"
#include "SoundComponent.hpp"
#include "Direction.hpp"

#include <entityplus/entity.h>
#include <entityplus/event.h>


struct Controllable;
struct DirectionChanged;
struct Jumped;
struct Knockback;
struct ChangeState;
struct StateChanged;

using Components = entityplus::component_list<PositionComponent, StateComponent, PhysicsComponent, AIComponent, SpriteComponent, AnimationComponent, SoundComponent>;
using Tags = entityplus::tag_list<Controllable>;
using Entities = entityplus::entity_manager<Components, Tags>;
using Events = entityplus::event_manager<Components, Tags, DirectionChanged, Jumped, Knockback, ChangeState, StateChanged>;
using Entity = Entities::entity_t;


struct DirectionChanged
{
	Entity entity;
	Direction direction;
};

struct Jumped
{
	Entity entity;
};

struct Knockback
{
	Entity attacker;
	Entity victim;
};

struct ChangeState
{
	Entity entity;
	EntityState state;
};

struct StateChanged
{
	Entity entity; 
	EntityState state;
};
