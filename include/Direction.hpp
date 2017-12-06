/*
Copyright (c) 2017 InversePalindrome
Nihil - Direction.hpp
InversePalindrome.com
*/


#pragma once

#include <cstddef>


enum class Direction : std::size_t
{ 
	Right, Left, Up, Down, RightUp, RightDown, LeftUp, LeftDown, Size 
};
enum class DirectionType : std::size_t
{
	Horizontal, Vertical
};