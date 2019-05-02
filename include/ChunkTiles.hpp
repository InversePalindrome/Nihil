/*
Copyright (c) 2017 InversePalindrome
Nihil - ChunkTiles.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <array>
#include <vector>
#include <memory>


class ChunkTiles : public sf::Drawable
{
    using Tile = std::array<sf::Vertex, 4u>;

public:
    ChunkTiles(const sf::Texture& texture);

    void addTile(const Tile& tile);

    sf::Vector2u getTextureSize() const;

private:
    const sf::Texture& texture;
    std::vector<sf::Vertex> vertices;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
};