/*
Copyright (c) 2017 InversePalindrome
Nihil - AIComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <SFML/System/Vector2.hpp>

#include <utility>


class AIComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const AIComponent& component);
	using Path = std::pair<float, float>;

public:
	AIComponent(float visionRange, float initialX, float finalX);

	float getVisionRange() const;
	Path getPath() const;

	void setVisionRange(float visionRange);
	void setPath(const Path& path);

	void swapPathPoints();
	
private:
	float visionRange;
    Path path;
};

std::ostream& operator<<(std::ostream& os, const AIComponent& component);

