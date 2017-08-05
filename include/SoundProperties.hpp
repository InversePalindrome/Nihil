/*
Copyright (c) 2017 InversePalindrome
Nihil - SoundProperties.hpp
InversePalindrome.com
*/


#pragma once


struct SoundProperties
{
	SoundProperties(float volume, float pitch, float attenuation, float minDistance) :
		volume(volume),
		pitch(pitch),
		attenuation(attenuation),
		minDistance(minDistance)
	{
	}

	float volume;
	float pitch;
	float attenuation;
	float minDistance;
};