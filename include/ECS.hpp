/*
Copyright (c) 2017 InversePalindrome
Nihil - ECS.hpp
InversePalindrome.com
*/


#pragma once

#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "PositionComponent.hpp"
#include "EntityEvents.hpp"

#include <entityplus/entity.h>
#include <entityplus/event.h>


struct Controllable;

using Components = entityplus::component_list<PositionComponent, PhysicsComponent, SpriteComponent>;
using Tags = entityplus::tag_list<Controllable>;
using Entities = entityplus::entity_manager<Components, Tags>;
using Events = entityplus::event_manager<Components, Tags, DirectionChanged>;
using Entity = Entities::entity_t;