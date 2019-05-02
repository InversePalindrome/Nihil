/*
Copyright (c) 2017 InversePalindrome
Nihil - Layer.cpp
InversePalindrome.com
*/


#include "Layer.hpp"
#include "FilePaths.hpp"


Layer::Layer(const tmx::Map& map, std::size_t layerID, const sf::Vector2f& layerSize) :
    size(layerSize)
{
    const auto& layers = map.getLayers();

    const auto& tileSize = map.getTileSize();
    size.x = std::floor(size.x / tileSize.x) * tileSize.x;
    size.y = std::floor(size.y / tileSize.y) * tileSize.y;

    const auto & layer = layers[layerID].get();

    if (layer->getType() == tmx::Layer::Type::Tile)
    {
        createChunks(map, *dynamic_cast<tmx::TileLayer*>(layer));
    }

    const auto& mapSize = map.getBounds();
    globalBounds.width = mapSize.width;
    globalBounds.height = mapSize.height;
}

const sf::FloatRect& Layer::getGlobalBounds() const
{
    return this->globalBounds;
}

void Layer::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    this->updateVisibility(renderTarget.getView());

    for (const auto& chunk : this->visibleChunks)
    {
        renderTarget.draw(*chunk, states);
    }
}

void Layer::createChunks(const tmx::Map& map, const tmx::TileLayer& layer)
{
    const auto& tileSets = map.getTilesets();
    const auto& layerIDs = layer.getTiles();

    std::uint32_t maxID = std::numeric_limits<std::uint32_t>::max();
    std::vector<const tmx::Tileset*> usedTileSets;

    for (auto i = tileSets.rbegin(); i != tileSets.rend(); ++i)
    {
        for (const auto& tile : layerIDs)
        {
            if (tile.ID >= i->getFirstGID() && tile.ID < maxID)
            {
                usedTileSets.push_back(&(*i));
                break;
            }
        }
        maxID = i->getFirstGID();
    }

    sf::Image fallback;
    fallback.create(2, 2, sf::Color::Magenta);
    for (const auto ts : usedTileSets)
    {
        const Path::FilePath& fileName(ts->getImagePath());
        auto newTexture = std::make_unique<sf::Texture>();

        sf::Image image;

        if (!image.loadFromFile(Path::textures / fileName.path.leaf().string()))
        {
            newTexture->loadFromImage(fallback);
        }
        else
        {
            if (ts->hasTransparency())
            {
                auto transparency = ts->getTransparencyColour();
                image.createMaskFromColor({ transparency.r, transparency.g, transparency.b, transparency.a });
            }
            newTexture->loadFromImage(image);
        }

        this->textures.emplace(fileName.path.string(), std::move(newTexture));
    }

    const auto& bounds = map.getBounds();
    this->chunkCount.x = static_cast<sf::Uint32>(std::ceil(bounds.width / this->size.x));
    this->chunkCount.y = static_cast<sf::Uint32>(std::ceil(bounds.height / this->size.y));

    sf::Vector2f tileCount(this->size.x / map.getTileSize().x, this->size.y / map.getTileSize().y);

    for (auto y = 0u; y < this->chunkCount.y; ++y)
    {
        for (auto x = 0u; x < this->chunkCount.x; ++x)
        {
            this->chunks.emplace_back(std::make_unique<Chunk>(layer, usedTileSets,
                sf::Vector2f(x * this->size.x, y * this->size.y), tileCount, map.getTileCount().x, this->textures));
        }
    }
}

void Layer::updateVisibility(const sf::View& view) const
{
    sf::Vector2f viewCorner = view.getCenter();
    viewCorner -= view.getSize() / 2.f;

    int posX = static_cast<int>(std::floor(viewCorner.x / this->size.x));
    int posY = static_cast<int>(std::floor(viewCorner.y / this->size.y));

    std::vector<const Chunk*> visible;

    for (auto y = posY; y < posY + 2; ++y)
    {
        for (auto x = posX; x < posX + 2; ++x)
        {
            auto id = y * this->chunkCount.x + x;

            if (id >= 0 && id < this->chunks.size() && !this->chunks.at(id)->isEmpty())
            {
                visible.push_back(this->chunks[id].get());
            }
        }
    }

    std::swap(this->visibleChunks, visible);
}