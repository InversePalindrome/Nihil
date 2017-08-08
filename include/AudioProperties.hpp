/*
Copyright (c) 2017 InversePalindrome
Nihil - AudioProperties.hpp
InversePalindrome.com
*/


#pragma once


struct AudioProperties
{
	AudioProperties(float volume, float pitch, float attenuation, float minDistance) :
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