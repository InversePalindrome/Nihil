/*
Copyright (c) 2017 InversePalindrome
Nihil - UnitConverter.hpp
InversePalindrome.com
*/


#pragma once


namespace UnitConverter
{
	static const float pixelsPerMeter = 16.f;

	static float pixelsToMeters(float pixels)
	{
		return pixels / pixelsPerMeter;
	}

	static float metersToPixels(float meters)
	{
		return meters * pixelsPerMeter;
	}
};