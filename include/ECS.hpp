/*
Copyright (c) 2017 InversePalindrome
Nihil - ECS.hpp
InversePalindrome.com
*/


#include "SpriteComponent.hpp"
#include "PositionComponent.hpp"

#include <entityplus/entity.h>
#include <entityplus/event.h>

#pragma once


using Components = entityplus::component_list<PositionComponent, SpriteComponent>;
using Tags = entityplus::tag_list<>;
using Entities = entityplus::entity_manager<Components, Tags>;
using Events = entityplus::event_manager<Components, Tags>;
using Entity = Entities::entity_t;