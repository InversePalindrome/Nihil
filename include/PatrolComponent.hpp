/*
Copyright (c) 2017 InversePalindrome
Nihil - PatrolComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <SFML/System/Vector2.hpp>

#include <utility>


class PatrolComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const PatrolComponent& component);
	using Path = std::pair<float, float>;

public:
	PatrolComponent(float initialX, float finalX);

	Path getPath() const;

	void setPath(const Path& path);
	void swapPathPoints();
	
private:
    Path path;
};

std::ostream& operator<<(std::ostream& os, const PatrolComponent& component);

