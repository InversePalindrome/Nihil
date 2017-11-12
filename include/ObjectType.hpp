/*
Copyright (c) 2017 InversePalindrome
Nihil - ObjectType.hpp
InversePalindrome.com
*/


#pragma once

#include <type_traits>


enum class ObjectType : std::size_t
{
	Tile = (1 << 0), Player = (1 << 1), Spike = (1 << 2), Enemy = (1 << 3), Portal = (1 << 4), Pickup = (1 << 5), Trampoline = (1 << 6),
	Border = (1 << 7), Platform = (1 << 8), Waypoint = (1 << 9), Bullet = (1 << 10), Bomb = (1 << 11), Explosion = (1 << 12), Teleporter = (1 << 13), 
	Character = (1 << 14), Liquid = (1 << 15), Feet = (1 << 16), Checkpoint = (1 << 17),
	Alive = Player | Enemy,
	Projectile = Bullet | Bomb,
	Lethal = Bullet | Explosion,
	Movable = Player | Enemy | Projectile,
	Block = Tile | Platform 
};

using T = std::underlying_type_t<ObjectType>;

constexpr ObjectType operator|(const ObjectType objectA, const ObjectType objectB)
{
	return static_cast<ObjectType>(static_cast<T>(objectA) | static_cast<T>(objectB));
}

constexpr T operator&(const ObjectType objectA, const ObjectType objectB)
{
	return static_cast<T>(objectA) & static_cast<T>(objectB);
}