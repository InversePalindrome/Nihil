/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationParser.cpp
InversePalindrome.com
*/


#include "AnimationParser.hpp"


void Parsers::parseFrameAnimations(const std::string& pathFile, thor::FrameAnimation& animation, 
	std::size_t& animationID, float& animationTime)
{
	std::ifstream inFile(pathFile);
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
			std::size_t left = 0u, top = 0u, width = 0u, length = 0u;

			iStream >> frameTime >> left >> top >> width >> length;

			animation.addFrame(frameTime, sf::IntRect(left, top, width, length));
		}
	}
}


thor::ColorGradient Parsers::parseColors(const std::string& pathFile)
{
	thor::ColorGradient colors;

	std::ifstream inFile(pathFile);

	float gradientPosition = 0.f;
	std::size_t R = 0u, G = 0u, B = 0u;

	while (inFile >> gradientPosition >> R >> G >> B)
	{
		colors[gradientPosition] = sf::Color(R, G, B);
	}

	return colors;
}