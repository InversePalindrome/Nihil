/*
Copyright (c) 2017 InversePalindrome
Nihil - Map.hpp
InversePalindrome.com
*/


#pragma once

#include "Layer.hpp"

#include <tmxlite/Map.hpp>

#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>


class Map : public sf::Drawable
{
public:
	Map(const std::string& filePath, const sf::Vector2f& chunkSize);

	void load(const std::string& filePath);

private:
	tmx::Map map;
	sf::Vector2f chunkSize;
	std::vector<std::unique_ptr<Layer>> layers;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
