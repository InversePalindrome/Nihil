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

	loadDataBitsets(levels, levelsBitset);
	loadDataBitsets(characters, charactersBitset);
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

std::unordered_map<std::string, bool>& Game::getLevels()
{
	return this->levels;
}

std::unordered_map<std::string, bool>& Game::getCharacters()
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
		os << level.second;
	}

	os << ' ';

	for (const auto& character : game.characters)
	{
		os << character.second;
	}

	return os;
}

void Game::loadNames()
{
	std::ifstream inFile("Resources/Files/Levels.txt");
	std::string line;

	bool isFirstElement = true;

	while (std::getline(inFile, line))
	{
		if (isFirstElement)
		{
			this->currentLevel = line;

			isFirstElement = false;
		}

		this->levels.emplace(line, false);
	}

	inFile.close();
	inFile.clear();

	isFirstElement = true;

	inFile.open("Resources/Files/Characters.txt");

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string name;

		iStream >> name;

		this->characters.emplace(name, false);

		if (isFirstElement)
		{
			this->characterName = name;
			this->characters[name] = true;

			isFirstElement = false;
		}
	}
}

void Game::loadDataBitsets(std::unordered_map<std::string, bool>& dataset, const boost::dynamic_bitset<std::size_t>& bitset)
{
	std::size_t i = bitset.size() - 1u;

	for (auto& data : dataset)
	{
		data.second = bitset[i];

		--i;
	}
}