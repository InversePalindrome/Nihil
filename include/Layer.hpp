/*
Copyright (c) 2017 InversePalindrome
Nihil - Layer.hpp
InversePalindrome.com
*/


#pragma once

#include "Chunk.hpp"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <map>
#include <memory>


class Layer : public sf::Drawable
{
public:
    Layer(const tmx::Map& map, std::size_t layerID, const sf::Vector2f& layerSize);

    const sf::FloatRect& getGlobalBounds() const;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;

private:
    sf::Vector2f size;
    sf::Vector2u chunkCount;
    sf::FloatRect globalBounds;

    std::map<std::string, std::unique_ptr<sf::Texture>> textures;

    std::vector<std::unique_ptr<Chunk>> chunks;
    mutable std::vector<const Chunk*> visibleChunks;

    void createChunks(const tmx::Map& map, const tmx::TileLayer& layer);

    void updateVisibility(const sf::View& view) const;
};
