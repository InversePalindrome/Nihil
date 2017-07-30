/*
Copyright (c) 2017 InversePalindrome
Nihil - Chunk.hpp
InversePalindrome.com
*/


#pragma once

#include "ChunkTiles.hpp"

#include <tmxlite/Tileset.hpp>
#include <tmxlite/TileLayer.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <map>
#include <array>
#include <memory>


class Chunk : public sf::Transformable, public sf::Drawable
{
	using Tile = std::array<sf::Vertex, 4u>;
	using Textures = std::map<std::string, std::unique_ptr<sf::Texture>>;

public:
	Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
		const sf::Vector2f& position, const sf::Vector2f& tileCount, std::size_t rowSize, Textures& textureResource);

	bool isEmpty() const;

private:
	std::vector<std::unique_ptr<ChunkTiles>> chunkTilesContainer;

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};
