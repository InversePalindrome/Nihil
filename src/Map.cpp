/*
Copyright (c) 2017 InversePalindrome
Nihil - Map.cpp
InversePalindrome.com
*/


#include "Map.hpp"


Map::Map(const std::string& filePath, const sf::Vector2f& chunkSize) :
	chunkSize(chunkSize)
{
	load(filePath);
}

void Map::load(const std::string& filePath)
{
	this->layers.clear();

	map.load(filePath);

	for (std::size_t i = 0; i < this->map.getLayers().size(); ++i)
	{
		layers.push_back(std::make_unique<Layer>(map, i, this->chunkSize));
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& layer : this->layers)
	{
		layer->draw(target, states);
	}
}