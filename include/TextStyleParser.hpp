/*
Copyright (c) 2017 InversePalindrome
Nihil - TextStyleParser.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"

#include <SFML/Graphics/Text.hpp>


namespace Path
{
	void parseStyle(ResourceManager& resourceManager, const std::string& filePath, sf::Text& text);
}