/*
Copyright (c) 2017 InversePalindrome
Nihil - AIComponent.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/System/Vector2.hpp>

#include <utility>


class AIComponent
{
	using Path = std::pair<float, float>;

public:
	AIComponent(float visionRange, float attackTime, float initialX, float finalX);

	float getVisionRange() const;
	Path getPath() const;

	void setVisionRange(float visionRange);
	void setPath(const Path& path);

	void swapPathPoints();
	
private:
	float visionRange;
    Path path;
};
