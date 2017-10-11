/*
Copyright (c) 2017 InversePalindrome
Nihil - Item.hpp
InversePalindrome.com
*/


#pragma once

#include <cstddef>
#include <unordered_map>


enum class Item
{
	Coin, OrangeGem, RedGem, PurpleGem, BlueGem, YellowGem, WhiteGem, SpeedBoost, JumpBoost
};

using Items = std::unordered_map<Item, std::size_t>;