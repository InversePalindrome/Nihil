/*
Copyright (c) 2017 InversePalindrome
Nihil - SpriteParser.cpp
InversePalindrome.com
*/


#include "SpriteParser.hpp"
#include "FilePaths.hpp"

#include <fstream>
#include <sstream>


void Parsers::parseSprite(ResourceManager& resourceManager, const std::string& fileName, sf::Sprite& sprite)
{
	std::ifstream inFile(Path::miscellaneous / fileName);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);
		
		std::string category;

		iStream >> category;

		if (category == "Texture")
		{
			std::size_t textureID = 0u;

			iStream >> textureID;

			sprite.setTexture(resourceManager.getTexture(static_cast<TexturesID>(textureID)));
		}
		else if (category == "SubRect")
		{
			std::size_t left = 0u, top = 0u, width = 0u, height = 0u;

			iStream >> left >> top >> width >> height;

			sprite.setTextureRect(sf::IntRect(left, top, width, height));
		}
		else if (category == "Scale")
		{
			float xScale = 0.f, yScale = 0.f;

			iStream >> xScale >> yScale;

			sprite.setScale(xScale, yScale);
		}
	}
}