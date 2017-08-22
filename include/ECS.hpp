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
#include "HealthComponent.hpp"
#include "AttackComponent.hpp"
#include "AnimationComponent.hpp"
#include "SoundComponent.hpp"
#include "ParticleComponent.hpp"
#include "ParentComponent.hpp"
#include "ChildComponent.hpp"
#include "Direction.hpp"

#include <entityplus/entity.h>
#include <entityplus/event.h>


struct Controllable;
struct Pickup;
struct DirectionChanged;
struct Jumped;
struct CombatOcurred;
struct ChangeState;
struct StateChanged;
struct FinishedLevel;
struct DestroyEntity;
struct EmitSound;
struct PickedUpCoin;
struct TouchedTrampoline;
struct GameOver;

using Components = entityplus::component_list<PositionComponent, StateComponent, 
	PhysicsComponent, AIComponent, HealthComponent, AttackComponent, SpriteComponent, AnimationComponent, SoundComponent,
    ParticleComponent, ParentComponent, ChildComponent>;

using Tags = entityplus::tag_list<Controllable, Pickup>;

using Entities = entityplus::entity_manager<Components, Tags>;

using Events = entityplus::event_manager<Components, Tags, DirectionChanged, Jumped, CombatOcurred, ChangeState,
	StateChanged, FinishedLevel, DestroyEntity, EmitSound, PickedUpCoin, TouchedTrampoline, GameOver>;

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

struct CombatOcurred
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

struct FinishedLevel
{
};

struct DestroyEntity
{
	Entity entity;
};

struct EmitSound
{
	SoundBuffersID soundBuffer;
	bool loop;
};

struct PickedUpCoin
{
};

struct TouchedTrampoline
{
	Entity entity;
};

struct GameOver
{
};