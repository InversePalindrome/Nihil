/*
Copyright (c) 2017 InversePalindrome
Nihil - GUIParser.cpp
InversePalindrome.com
*/


#include "GUIParser.hpp"
#include "FilePaths.hpp"

#include <fstream>
#include <sstream>


void Parsers::parseGUIProperties(GUIManager& guiManager, const std::string& fileName)
{
	std::ifstream inFile(Path::GUI / fileName);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string selector;
		std::string category;

		auto numberOfWords = std::distance(std::istream_iterator<std::string>(iStream), std::istream_iterator<std::string>());

		iStream.clear();
		iStream.str(line);

		iStream >> selector >> category;

		if (category == "FontName")
		{
			std::string font;
			
			iStream >> font;

			guiManager.setProperty(selector, category, Path::fonts / font);
		}
		else if (numberOfWords == 3)
		{
			float floatValue = 0.f;

			iStream >> floatValue;

			guiManager.setProperty(selector, category, floatValue);
		}
		else if (numberOfWords == 5)
		{
			float R = 0.f, G = 0.f, B = 0.f;

			iStream >> R >> G >> B;

			guiManager.setProperty(selector, category, sf::Color(R, G, B));
		}
	}
}