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
#include "PatrolComponent.hpp"
#include "HealthComponent.hpp"
#include "AttackComponent.hpp"
#include "AnimationComponent.hpp"
#include "SoundComponent.hpp"
#include "ParticleComponent.hpp"
#include "ParentComponent.hpp"
#include "ChildComponent.hpp"
#include "AutomatedComponent.hpp"
#include "ControllableComponent.hpp"
#include "ChaseComponent.hpp"
#include "TimerComponent.hpp"
#include "Direction.hpp"

#include <brigand/sequences/list.hpp>

#include <entityplus/entity.h>
#include <entityplus/event.h>


struct DirectionChanged;
struct Jumped;
struct StopMovement;
struct CombatOcurred;
struct ChangeState;
struct StateChanged;
struct Teleported;
struct DestroyEntity;
struct EmitSound;
struct PickedUpCoin;
struct TouchedTrampoline;
struct MoveCamera;
struct DisplayHealthBar;
struct PlayerDied;
struct CrossedWaypoint;


using Components = entityplus::component_list<PositionComponent, StateComponent, 
	PhysicsComponent, PatrolComponent, HealthComponent, AttackComponent, SpriteComponent, AnimationComponent, SoundComponent,
    ParticleComponent, ParentComponent, ChildComponent, AutomatedComponent, ControllableComponent, ChaseComponent, TimerComponent>;

using Tags = entityplus::tag_list<>;

using ComponentList = brigand::list<PositionComponent, StateComponent, PhysicsComponent, PatrolComponent,
	HealthComponent, AttackComponent, SpriteComponent, AnimationComponent, SoundComponent,
	ParticleComponent, ParentComponent, ChildComponent, AutomatedComponent, ControllableComponent, ChaseComponent, TimerComponent>;

using Entities = entityplus::entity_manager<Components, Tags>;

using Events = entityplus::event_manager<Components, Tags, DirectionChanged, Jumped, StopMovement, CombatOcurred, ChangeState,
	StateChanged, Teleported, DestroyEntity, EmitSound, PickedUpCoin, TouchedTrampoline, MoveCamera, DisplayHealthBar,
    PlayerDied, CrossedWaypoint>;

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

struct StopMovement
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

struct Teleported
{
	std::string level;
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

struct MoveCamera
{
	Entity entity;
};

struct DisplayHealthBar
{
	Entity entity;
};

struct PlayerDied
{
};

struct CrossedWaypoint
{
	Entity entity;
};