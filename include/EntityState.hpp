/*
Copyright (c) 2017 InversePalindrome
Nihil - EntityState.hpp
InversePalindrome.com
*/


#pragma once

#include <cstddef>


enum class EntityState : std::size_t
{
    Idle, Walking, Jumping, Attacking
};