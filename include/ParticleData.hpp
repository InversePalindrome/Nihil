/*
Copyright (c) 2017 InversePalindrome
Nihil - ParticleData.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/Graphics/Color.hpp>


struct ParticleData
{
	ParticleData() :
		ParticleData(0.f, 0.f, sf::Color())
	{
	}

	ParticleData(float emissionRate, float particleLifetime, sf::Color& particleColor) :
		emissionRate(emissionRate),
		particleLifetime(particleLifetime),
		particleColor(particleColor)
	{
	}

	float emissionRate;
	float particleLifetime;
	
	sf::Color particleColor;
};