/*
Copyright (c) 2017 InversePalindrome
Nihil - EffectParser.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"

#include <string>

#include <Thor/Particles/Emitters.hpp>
#include <Thor/Particles/ParticleSystem.hpp>


namespace Parsers
{
	void parseParticleSystem(ResourceManager& resourceManager, const std::string& fileName, thor::ParticleSystem& particleSystem);
	thor::UniversalEmitter parseEmitter(const std::string& fileName);
}