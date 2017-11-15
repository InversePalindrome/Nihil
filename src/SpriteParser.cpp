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
	std::ifstream inFile(Path::spriteInfo / fileName);
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
			int left = 0, top = 0, width = 0, height = 0;

			iStream >> left >> top >> width >> height;

			sprite.setTextureRect({ left, top, width, height });
		}
		else if (category == "Scale")
		{
			float xScale = 0.f, yScale = 0.f;

			iStream >> xScale >> yScale;

			sprite.setScale(xScale, yScale);
		}
		else if (category == "Rotate")
		{
			float angle = 0.f;

			iStream >> angle;

			sprite.setRotation(angle);
		}
	}
}