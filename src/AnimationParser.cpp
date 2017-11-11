/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationParser.cpp
InversePalindrome.com
*/


#include "AnimationParser.hpp"
#include "FilePaths.hpp"


void Parsers::parseFrameAnimations(const std::string& fileName, thor::FrameAnimation& animation, 
	std::size_t& animationID, float& animationTime)
{
	std::ifstream inFile(Path::animations / fileName);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string category;

		iStream >> category;

		if (category == "Animation")
		{
			iStream >> animationID >> animationTime;
		}
		else if (category == "Frame")
		{
			float frameTime = 0.f;
			int left = 0, top = 0, width = 0, length = 0;

			iStream >> frameTime >> left >> top >> width >> length;

			animation.addFrame(frameTime, { left, top, width, length });
		}
	}
}

thor::ColorGradient Parsers::parseColors(const std::string& fileName)
{
	thor::ColorGradient colors;

	std::ifstream inFile(Path::miscellaneous / fileName);

	float gradientPosition = 0.f;
	std::size_t R = 0u, G = 0u, B = 0u;

	while (inFile >> gradientPosition >> R >> G >> B)
	{
		colors[gradientPosition] = sf::Color(static_cast<sf::Uint8>(R), static_cast<sf::Uint8>(G), static_cast<sf::Uint8>(B));
	}

	return colors;
}