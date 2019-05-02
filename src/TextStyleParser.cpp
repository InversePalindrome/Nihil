/*
Copyright (c) 2017 InversePalindrome
Nihil - TextStyleParser.hpp
InversePalindrome.com
*/


#include "TextStyleParser.hpp"
#include "FilePaths.hpp"

#include <fstream>
#include <sstream>


void Parsers::parseStyle(ResourceManager& resourceManager, const std::string& fileName, sf::Text& text)
{
    std::ifstream inFile(Path::miscellaneous / fileName);
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

            text.setFont(resourceManager.getFont(FontsID{ fontID }));
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

            text.setFillColor(sf::Color(static_cast<sf::Uint8>(R), static_cast<sf::Uint8>(G), static_cast<sf::Uint8>(B)));
        }
        else if (category == "BorderColor")
        {
            std::size_t R = 0u, G = 0u, B = 0u;

            iStream >> R >> G >> B;

            text.setOutlineColor(sf::Color(static_cast<sf::Uint8>(R), static_cast<sf::Uint8>(G), static_cast<sf::Uint8>(B)));
        }
    }
}