/*
Copyright (c) 2017 InversePalindrome
Nihil - TextStyleParser.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"

#include <SFML/Graphics/Text.hpp>


namespace TextStyleParser
{
	void parseStyle(ResourceManager& resourceManager, const std::string& pathFile, sf::Text& text);
}