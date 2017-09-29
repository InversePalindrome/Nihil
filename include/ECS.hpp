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
#include "MeleeAttackComponent.hpp"
#include "RangeAttackComponent.hpp"
#include "BulletComponent.hpp"
#include "BombComponent.hpp"
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


struct AI;

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
struct MoveCamera;
struct DisplayHealthBar;
struct PlayerDied;
struct CrossedWaypoint;
struct ShootProjectile;
struct ActivateBomb;
struct BombExploded;
struct CreateTransform;
struct ApplyForce;
struct ApplyImpulse;
struct ApplyBlastImpact;
struct ApplyKnockback;


using Components = entityplus::component_list<PositionComponent, StateComponent, PhysicsComponent, PatrolComponent, TimerComponent,
	HealthComponent, MeleeAttackComponent, RangeAttackComponent, BulletComponent, BombComponent, SpriteComponent, AnimationComponent, 
	SoundComponent, ParticleComponent, ParentComponent, ChildComponent, AutomatedComponent, ControllableComponent, ChaseComponent>;

using Tags = entityplus::tag_list<AI>;

using Entities = entityplus::entity_manager<Components, Tags>;

using Events = entityplus::event_manager<Components, Tags, DirectionChanged, Jumped, StopMovement, CombatOcurred, ChangeState,
	StateChanged, Teleported, DestroyEntity, EmitSound, PickedUpCoin, MoveCamera, DisplayHealthBar, PlayerDied, CrossedWaypoint,
	ShootProjectile, ActivateBomb, BombExploded, CreateTransform, ApplyForce, ApplyImpulse, ApplyBlastImpact, ApplyKnockback>;

using Entity = Entities::entity_t;

using ComponentList = brigand::list<PositionComponent, StateComponent, PhysicsComponent, PatrolComponent, TimerComponent,
	HealthComponent, MeleeAttackComponent, RangeAttackComponent, BulletComponent, BombComponent, SpriteComponent, AnimationComponent,
	SoundComponent, ParticleComponent, ParentComponent, ChildComponent, AutomatedComponent, ControllableComponent, ChaseComponent>;


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

struct ShootProjectile
{
	Entity shooter;
	std::string projectileID; 
	sf::Vector2f targetPosition;
};

struct ActivateBomb
{
	Entity bomb;
};

struct BombExploded
{
	Entity bomb;
	Entity explosion;
};

struct CreateTransform
{
	Entity childEntity;
	ChildComponent& child;
	ParentComponent& parent;
};

struct ApplyForce
{
	Entity entity;
	b2Vec2 force;
};

struct ApplyImpulse
{
	Entity entity;
	b2Vec2 impulse;
};

struct ApplyBlastImpact
{
	Entity explosion;
	Entity victim;
};

struct ApplyKnockback
{
	Entity attacker;
	Entity victim;
};