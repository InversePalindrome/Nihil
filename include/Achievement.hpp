/*
Copyright (c) 2017 InversePalindrome
Nihil - Achievement.hpp
InversePalindrome.com
*/


#pragma once

#include <unordered_map>


enum class Achievement { Annihilator, Traveler, Collector, BigSpender};

using Achievements = std::unordered_map<Achievement, std::pair<std::size_t, std::size_t>>;