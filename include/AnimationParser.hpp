/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationParser.hpp
InversePalindrome.com
*/


#pragma once

#include "Animation.hpp"

#include <string>


namespace Parsers
{
    Animations parseAnimations(const std::string& fileName, Animator& Animator);
}