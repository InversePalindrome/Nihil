/*
Copyright (c) 2017 InversePalindrome
Nihil - TextStyleParser.hpp
InversePalindrome.com
*/


#include "TextStyleParser.hpp"

#include <fstream>
#include <sstream>


void TextStyleParser::parseStyle(ResourceManager& resourceManager, const std::string& pathFile, sf::Text& text)
{
	std::ifstream inFile(pathFile);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string category;

		iStream >> category;

		if (category == "Font")
		{
			std::size_t fontID = 0u;

			iStream >> fontID;

			text.setFont(resourceManager.getFont(static_cast<FontsID>(fontID)));
		}
		else if (category == "CharacterSize")
		{
			std::size_t characterSize = 0u;

			iStream >> characterSize;

			text.setCharacterSize(characterSize);
		}
		else if (category == "BorderThickness")
		{
			float thickness = 0.f;

			iStream >> thickness;

			text.setOutlineThickness(thickness);
		}
		else if (category == "Color")
		{
			std::size_t R = 0u, G = 0u, B = 0u;

			iStream >> R >> G >> B;

			text.setFillColor(sf::Color(R, G, B));
		}
		else if (category == "BorderColor")
		{
			std::size_t R = 0u, G = 0u, B = 0u;

			iStream >> R >> G >> B;

			text.setOutlineColor(sf::Color(R, G, B));
		}
	}
}