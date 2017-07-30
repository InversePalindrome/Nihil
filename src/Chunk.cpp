/*
Copyright (c) 2017 InversePalindrome
Nihil - Chunk.cpp
InversePalindrome.com
*/


#include "Chunk.hpp"


Chunk::Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
	const sf::Vector2f& position, const sf::Vector2f& tileCount, std::size_t rowSize, Textures& textures)
{
	auto opacity = static_cast<sf::Uint8>(layer.getOpacity() / 1.f * 255.f);
	sf::Color vertexColor = sf::Color::White;
	vertexColor.a = opacity;

	auto offset = layer.getOffset();
	sf::Vector2f layerOffset(offset.x, offset.y);

	const auto& tileIDs = layer.getTiles();

	for (const auto ts : tilesets)
	{
		bool chunkArrayCreated = false;
		auto tileSize = ts->getTileSize();

		sf::Vector2u tsTileCount;

		std::size_t xPos = static_cast<std::size_t>(position.x / tileSize.x);
		std::size_t yPos = static_cast<std::size_t>(position.y / tileSize.y);

		for (auto y = yPos; y < yPos + tileCount.y; ++y)
		{
			for (auto x = xPos; x < xPos + tileCount.x; ++x)
			{
				auto idx = (y * rowSize + x);

				if (idx < tileIDs.size() && tileIDs[idx].ID >= ts->getFirstGID()
					&& tileIDs[idx].ID < (ts->getFirstGID() + ts->getTileCount()))
				{
					if (!chunkArrayCreated)
					{
						this->chunkTilesContainer.emplace_back(std::make_unique<ChunkTiles>(*textures.find(ts->getImagePath())->second));
						auto texSize = chunkTilesContainer.back()->getTextureSize();
						tsTileCount.x = texSize.x / tileSize.x;
						tsTileCount.y = texSize.y / tileSize.y;
						chunkArrayCreated = true;
					}

					auto& ChunkContainer = chunkTilesContainer.back();

					sf::Vector2f tileOffset(x * tileSize.x, y * tileSize.y);

					auto idIndex = tileIDs[idx].ID - ts->getFirstGID();
					sf::Vector2f tileIndex(idIndex % tsTileCount.x, idIndex / tsTileCount.x);
					tileIndex.x *= tileSize.x;
					tileIndex.y *= tileSize.y;

					Tile tile =
					{
						sf::Vertex(tileOffset, vertexColor, tileIndex),
						sf::Vertex(tileOffset + sf::Vector2f(tileSize.x, 0.f), vertexColor, tileIndex + sf::Vector2f(tileSize.x, 0.f)),
						sf::Vertex(tileOffset + sf::Vector2f(tileSize.x, tileSize.y), vertexColor, tileIndex + sf::Vector2f(tileSize.x, tileSize.y)),
						sf::Vertex(tileOffset + sf::Vector2f(0.f, tileSize.y), vertexColor, tileIndex + sf::Vector2f(0.f, tileSize.y))
					};
					ChunkContainer->addTile(tile);
				}
			}
		}
	}

	setPosition(position);
}

bool Chunk::isEmpty() const
{
	return this->chunkTilesContainer.empty();
}

void Chunk::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (const auto& chunkContainer : this->chunkTilesContainer)
	{
		renderTarget.draw(*chunkContainer, states);
	}
}