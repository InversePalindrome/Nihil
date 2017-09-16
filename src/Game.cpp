/*
Copyright (c) 2017 InversePalindrome
Nihil - Game.cpp
InversePalindrome.com
*/


#include "Game.hpp"

#include <fstream>
#include <sstream>


Game::Game() :
	coins(0u)
{
	loadNames();

	std::ifstream inFile("Resources/Files/Levels.txt");

	inFile >> currentLevel;

	inFile.close();
	inFile.clear();

	inFile.open("Resources/Files/Characters.txt");

	inFile >> characterName;

	characters.modify(std::begin(characters), [](auto& character) { character.isLoaded = true; });

	loadSpawnPoints();
}

Game::Game(const std::string& data) :
	coins(0u)
{
	std::istringstream iStream(data);

	std::string levelBits, characterBits;

	iStream >> gameName >> characterName >> currentLevel >> coins >> levelBits >> characterBits;

	loadNames();

    const auto& levelsBitset = boost::dynamic_bitset<std::size_t>(levelBits);
	const auto& charactersBitset = boost::dynamic_bitset<std::size_t>(characterBits);

	loadDataBitsets<LoadedLevels>(levels, levelsBitset);
	loadDataBitsets<LoadedCharacters>(characters, charactersBitset);

	loadSpawnPoints();
}

std::string Game::getGameName() const
{
	return this->gameName;
}

std::string Game::getCharacterName() const
{
	return this->characterName;
}

std::string Game::getCurrentLevel() const
{
	return this->currentLevel;
}

std::size_t Game::getCoins() const
{
	return this->coins;
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

void Game::setCharacterName(const std::string& characterName)
{
	this->characterName = characterName;
}

void Game::setCurrentLevel(const std::string& currentLevel)
{
	this->currentLevel = currentLevel;
}

void Game::setCoins(std::size_t coins)
{
	this->coins = coins;
}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
	os << game.gameName << ' ' << game.characterName << ' ' << game.currentLevel << ' ' << game.coins << ' ';
	
	for (const auto& level : game.levels)
	{
		os << level.isLoaded;
	}

	os << ' ';

	for (const auto& character : game.characters)
	{
		os << character.isLoaded;
	}

	return os;
}

void Game::loadNames()
{
	std::ifstream inFile("Resources/Files/Levels.txt");
	std::string line;

	while (std::getline(inFile, line))
	{
		this->levels.get<1>().insert({ line, sf::Vector2f(), false });
	}

	inFile.close();
	inFile.clear();

	inFile.open("Resources/Files/Characters.txt");

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string name;

		iStream >> name;

		this->characters.get<1>().insert({ name, false });
	}
}

void Game::loadSpawnPoints()
{
	for (const auto& level : this->levels)
	{
		std::ifstream inFile("Resources/Files/BlueprintObjects-" + level.name + ".txt");
		std::string line;

		while (std::getline(inFile, line))
		{
			std::istringstream iStream(line);

			std::int32_t entityID;
			std::string pathFile;

			iStream >> entityID >> pathFile;

			if (pathFile == "Resources/Files/Player.txt")
			{
				float xPosition = 0.f, yPosition = 0.f;
				
				iStream >> xPosition >> yPosition;
				
				this->levels.get<1>().modify(this->levels.get<1>().find(level.name), [xPosition, yPosition](auto& iLevel) { iLevel.spawnPosition = sf::Vector2f(xPosition, yPosition); });
			}
		}
	}
}