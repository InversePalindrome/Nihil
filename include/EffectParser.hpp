/*
Copyright (c) 2017 InversePalindrome
Nihil - EffectParser.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"

#include <Thor/Particles/Emitters.hpp>
#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Animations/ColorAnimation.hpp>

#include <string>


namespace Parsers
{
    void parseParticleSystem(ResourceManager& resourceManager, const std::string& fileName, thor::ParticleSystem& particleSystem);
    thor::UniversalEmitter parseEmitter(const std::string& fileName);

    thor::ColorGradient parseColors(const std::string& fileName);
}