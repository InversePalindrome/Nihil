/*
Copyright (c) 2017 InversePalindrome
Nihil - ChunkContainer.cpp
InversePalindrome.com
*/


#include "ChunkTiles.hpp"


ChunkTiles::ChunkTiles(const sf::Texture& texture) :
    texture(texture)
{
}

void ChunkTiles::addTile(const Tile& tile)
{
    for (const auto& vertex : tile)
    {
        this->vertices.push_back(vertex);
    }
}

sf::Vector2u ChunkTiles::getTextureSize() const
{
    return this->texture.getSize();
}

void ChunkTiles::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    states.texture = &this->texture;

    renderTarget.draw(this->vertices.data(), this->vertices.size(), sf::Quads, states);
}