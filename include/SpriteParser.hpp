/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteParser.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <string>


namespace Parsers
{
    void parseSprite(ResourceManager& resourceManager, const std::string& fileName, sf::Sprite& sprite);
}