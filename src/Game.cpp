/*
Copyright (c) 2017 InversePalindrome
Nihil - Game.cpp
InversePalindrome.com
*/


#include "Game.hpp"
#include "FilePaths.hpp"

#include <fstream>
#include <sstream>


Game::Game() 
{
	loadCharacterNames();
	loadLevelNames();
	loadSpawnPoints();

	std::ifstream inFile(Path::levels / "Levels.txt");

	inFile >> currentLevel;

	inFile.close();
	inFile.clear();

	inFile.open(Path::miscellaneous / "Characters.txt");

	inFile >> currentCharacter;

	characters.modify(std::begin(characters), [](auto& character) { character.isLoaded = true; });
}

Game::Game(const std::string& data) 
{
	std::istringstream iStream(data);
	std::string line;

	std::string levelBits, characterBits;

	while (std::getline(iStream, line))
	{
		std::istringstream inLine(line);

		std::string category;

		inLine >> category;

		if (category == "Game")
		{
			inLine >> gameName;
		}
		else if (category == "CurrentCharacter")
		{
			inLine >> currentCharacter;
		}
		else if (category == "CurrentLevel")
		{
			inLine >> currentLevel;
		}
		else if (category == "Characters")
		{
			inLine >> characterBits;
		}
		else if (category == "Levels")
		{
			inLine >> levelBits;
		}
		else if (category == "Item")
		{
			std::size_t item = 0u, quantity = 0u;

			inLine >> item >> quantity;

			this->items.emplace(static_cast<Item>(item), quantity);
		}
	}

	loadCharacterNames();
	loadLevelNames();
	loadSpawnPoints();

    const auto& levelsBitset = boost::dynamic_bitset<std::size_t>(levelBits);
	const auto& charactersBitset = boost::dynamic_bitset<std::size_t>(characterBits);

	loadDataBitsets<LoadedLevels>(levels, levelsBitset);
	loadDataBitsets<LoadedCharacters>(characters, charactersBitset);
}

std::string Game::getGameName() const
{
	return this->gameName;
}

std::string Game::getCurrentCharacter() const
{
	return this->currentCharacter;
}

std::string Game::getCurrentLevel() const
{
	return this->currentLevel;
}

Items& Game::getItems() 
{
	return this->items;
}

sf::Vector2f Game::getSpawnPoint() const
{
	return this->levels.get<1>().find(this->currentLevel)->spawnPosition;
}

Game::LoadedLevels& Game::getLevels()
{
	return this->levels;
}

Game::LoadedCharacters& Game::getCharacters()
{
	return this->characters;
}

void Game::setGameName(const std::string& gameName)
{
	this->gameName = gameName;
}

void Game::setCurrentCharacter(const std::string& currentCharacter)
{
	this->currentCharacter = currentCharacter;
}

void Game::setCurrentLevel(const std::string& currentLevel)
{
	this->currentLevel = currentLevel;
}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
	os << "Game " << game.gameName << '\n';
	os << "CurrentCharacter " << game.currentCharacter << '\n';
	os << "CurrentLevel " << game.currentLevel << '\n';
	os << "Characters ";

	for (const auto& character : game.characters)
	{
		os << character.isLoaded;
	}

	os << '\n';

	os << "Levels ";

	for (const auto& level : game.levels)
	{
		os << level.isLoaded;
	}
	
	os << '\n';

	for (const auto& item : game.items)
	{
		os << "Item " << static_cast<std::size_t>(item.first) << ' ' << item.second << '\n';
	}

	return os;
}

void Game::loadCharacterNames()
{
	std::ifstream inFile(Path::miscellaneous / "Characters.txt");
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string name;

		iStream >> name;

		this->characters.get<1>().insert({ name, false });
	}
}

void Game::loadLevelNames()
{
	std::ifstream inFile(Path::levels / "Levels.txt");
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);
		
	    std::string levelName;

		iStream >> levelName;

		this->levels.get<1>().insert({ levelName, sf::Vector2f(), false });
	}
}

void Game::loadSpawnPoints()
{
	std::ifstream inFile(Path::levels / "levels.txt ");
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string levelName;
		float xPosition = 0.f, yPosition = 0.f;

		iStream >> levelName >> xPosition >> yPosition;
				
		this->levels.get<1>().modify(this->levels.get<1>().find(levelName), [xPosition, yPosition](auto& iLevel) { iLevel.spawnPosition = { xPosition, yPosition }; });
	}
}