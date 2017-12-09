/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationParser.hpp
InversePalindrome.com
*/


#pragma once

#include "Animation.hpp"

#include <Thor/Animations/Animator.hpp>
#include <Thor/Animations/FrameAnimation.hpp>
#include <Thor/Animations/ColorAnimation.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>


namespace Parsers
{
    Animations parseAnimations(const std::string& fileName, Animator& Animator);

	thor::ColorGradient parseColors(const std::string& fileName);
}