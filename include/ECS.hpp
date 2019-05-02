/*
Copyright (c) 2017 InversePalindrome
Nihil - ECS.hpp
InversePalindrome.com
*/


#pragma once

#include "SpriteComponent.hpp"
#include "TextComponent.hpp"
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
#include "ParticleComponent.hpp"
#include "ParentComponent.hpp"
#include "ChildComponent.hpp"
#include "AutomatedComponent.hpp"
#include "ControllableComponent.hpp"
#include "ChaseComponent.hpp"
#include "TimerComponent.hpp"
#include "PickupComponent.hpp"
#include "PowerUpComponent.hpp"
#include "DropComponent.hpp"
#include "InventoryComponent.hpp"
#include "LockComponent.hpp"
#include "KeyComponent.hpp"
#include "DialogComponent.hpp"
#include "Direction.hpp"
#include "Achievement.hpp"
#include "Animation.hpp"

#include <brigand/sequences/list.hpp>

#include <entityplus/entity.h>
#include <entityplus/event.h>


struct AI;
struct Turret;

struct CreateEntity;
struct DestroyBody;
struct UpdateAchievement;
struct UpdateConversation;
struct ChangeDirection;
struct DirectionChanged;
struct Jumped;
struct StopMovement;
struct StopSound;
struct StopAnimation;
struct CombatOcurred;
struct ChangeState;
struct StateChanged;
struct ChangeLevel;
struct DestroyEntity;
struct PlaySound;
struct PlayAnimation;
struct PickedUpItem;
struct DroppedItem;
struct DisplayHealthBar;
struct DisplayCoins;
struct DisplayPowerUp;
struct DisplayConversation;
struct HidePowerUp;
struct CrossedCheckpoint;
struct CrossedWaypoint;
struct ShootProjectile;
struct ActivateBomb;
struct CreateTransform;
struct ApplyForce;
struct ApplyImpulse;
struct ApplyBlastImpact;
struct ApplyKnockback;
struct SetUserData;
struct SetGravityScale;
struct SetLinearDamping;
struct SetVelocity;
struct SetPosition;
struct SetAngle;
struct SetMidAirStatus;
struct SetUnderWaterStatus;
struct SetFriction;
struct AddUnderWaterTimer;
struct RemoveUnderWaterTimer;
struct PropelFromWater;
struct AddedUserData;
struct ManageCollision;

using Components = entityplus::component_list<PositionComponent, StateComponent, PhysicsComponent, PatrolComponent, TimerComponent,
    HealthComponent, MeleeAttackComponent, RangeAttackComponent, BulletComponent, BombComponent, SpriteComponent, TextComponent,
    AnimationComponent, ParticleComponent, ParentComponent, ChildComponent, AutomatedComponent, ControllableComponent, ChaseComponent,
    PickupComponent, PowerUpComponent, DropComponent, InventoryComponent, LockComponent, KeyComponent, DialogComponent>;

using Tags = entityplus::tag_list<AI, Turret>;

using Entities = entityplus::entity_manager<Components, Tags>;

using Events = entityplus::event_manager<Components, Tags, CreateEntity, DestroyBody, UpdateAchievement, UpdateConversation, ChangeDirection, DirectionChanged,
    Jumped, StopMovement, StopSound, StopAnimation, CombatOcurred, ChangeState, StateChanged, ChangeLevel, DestroyEntity, PlaySound, PlayAnimation, PickedUpItem,
    DroppedItem, DisplayHealthBar, DisplayCoins, DisplayPowerUp, DisplayConversation, HidePowerUp, CrossedCheckpoint, CrossedWaypoint, ShootProjectile, ActivateBomb,
    CreateTransform, ApplyForce, ApplyImpulse, ApplyBlastImpact, ApplyKnockback, SetUserData, SetGravityScale, SetLinearDamping, SetVelocity, SetPosition, SetAngle,
    SetMidAirStatus, SetUnderWaterStatus, SetFriction, AddUnderWaterTimer, RemoveUnderWaterTimer, PropelFromWater, AddedUserData, ManageCollision>;

using Entity = Entities::entity_t;

using ComponentList = brigand::list<PositionComponent, StateComponent, PhysicsComponent, PatrolComponent, TimerComponent,
    HealthComponent, MeleeAttackComponent, RangeAttackComponent, BulletComponent, BombComponent, SpriteComponent, TextComponent,
    AnimationComponent, ParticleComponent, ParentComponent, ChildComponent, AutomatedComponent, ControllableComponent, ChaseComponent,
    PickupComponent, PowerUpComponent, DropComponent, InventoryComponent, LockComponent, KeyComponent, DialogComponent>;

struct CreateEntity
{
    std::int32_t entityType;
    std::string fileName;
    sf::Vector2f position;
};

struct DestroyBody
{
    PhysicsComponent& physics;
};

struct UpdateAchievement
{
    Achievement achievement;
};

struct UpdateConversation
{
    Entity entity;
};

struct ChangeDirection
{
    Entity entity;
    Direction direction;
};

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

struct StopSound
{
    SoundBuffersID soundID;
};

struct StopAnimation
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

struct ChangeLevel
{
    std::string level;
    sf::Vector2f position;
};

struct DestroyEntity
{
    Entity entity;
};

struct PlaySound
{
    SoundBuffersID soundBuffer;
    bool loop;
};

struct PlayAnimation
{
    Entity entity;
    Animation animation;
    bool loop;
};

struct PickedUpItem
{
    Entity collector;
    Entity item;
};

struct DroppedItem
{
    Entity dropper;
};

struct GameOver
{
};

struct DisplayHealthBar
{
    const HealthComponent& health;
};

struct DisplayCoins
{
};

struct DisplayPowerUp
{
    Item item;
};

struct DisplayConversation
{
    Entity entity;
    bool visibilityStatus;
};

struct HidePowerUp
{
    Item item;
};

struct CrossedCheckpoint
{
    sf::Vector2f position;
};

struct CrossedWaypoint
{
    Entity entity;
};

struct ShootProjectile
{
    Entity shooter;
    std::string projectileID;
};

struct ActivateBomb
{
    Entity bomb;
};

struct CreateTransform
{
    Entity childEntity;
    Entity parentEntity;
    sf::Vector2f offset;
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

struct SetUserData
{
    Entity entity;
};

struct SetGravityScale
{
    Entity entity;
    float gravity;
};

struct SetLinearDamping
{
    Entity entity;
    float linearDamping;
};

struct SetVelocity
{
    Entity entity;
    Direction direction;
};

struct SetPosition
{
    Entity entity;
    sf::Vector2f position;
};

struct SetAngle
{
    Entity entity;
    float angle;
};

struct SetMidAirStatus
{
    Entity entity;
    bool midAirStatus;
};

struct SetUnderWaterStatus
{
    Entity entity;
    bool underWaterStatus;
};

struct SetFriction
{
    Entity entity;
    ObjectType fixtureType;
    float friction;
};

struct AddUnderWaterTimer
{
    Entity entity;
};

struct RemoveUnderWaterTimer
{
    Entity entity;
};

struct PropelFromWater
{
    Entity entity;
};

struct AddedUserData
{
    Entity entity;
};

struct ManageCollision
{
    Entity entityA;
    Entity entityB;
    bool collisionStatus;
};