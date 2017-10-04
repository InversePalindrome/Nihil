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


namespace Path
{
	void parseParticleSystem(ResourceManager& resourceManager, const std::string& filePath, thor::ParticleSystem& particleSystem);
	thor::UniversalEmitter parseEmitter(const std::string& filePath);
}