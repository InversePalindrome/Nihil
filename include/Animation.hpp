/*
Copyright (c) 2017 InversePalindrome
Nihil - Animation.hpp
InversePalindrome.com
*/


#pragma once

#include "EntityState.hpp"
#include "Direction.hpp"

#include <Thor/Animations/FrameAnimation.hpp>
#include <Thor/Animations/Animator.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <boost/functional/hash/hash.hpp>

#include <Utility>
#include <unordered_map>


using Animation = std::pair<EntityState, Direction>;
using Animations = std::unordered_map<Animation, std::pair<thor::FrameAnimation, float>, boost::hash<Animation>>;
using Animator = thor::Animator<sf::Sprite, Animation>;