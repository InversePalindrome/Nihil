/*
Copyright (c) 2017 InversePalindrome
Nihil - UnitConverter.hpp
InversePalindrome.com
*/


#pragma once

#include <cmath>

namespace UnitConverter
{
	template<typename T>
	static const T pixelsPerMeter = T(16);

	template<typename T>
	static const T pi = T(std::atan(1) * 4);

	template<typename T>
	auto pixelsToMeters(T pixels)
	{
		return pixels / pixelsPerMeter<T>;
	}

	template<typename T>
	auto metersToPixels(T meters)
	{
		return meters * pixelsPerMeter<T>;
	}

	template<typename T>
	auto degreesToRadians(T degrees)
	{
		return degrees * pi<T> / T(180);
	}

	template<typename T>
	auto radiansToDegrees(T radians)
	{
		return radians * T(180) / pi<T>;
	}
};