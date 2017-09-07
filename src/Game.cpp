/*
Copyright (c) 2017 InversePalindrome
Nihil - Game.cpp
InversePalindrome.com
*/


#include "Game.hpp"


Game::Game(const std::string& gameName) :
	Game(gameName, "Red", "City", 0u, {})
{
}

Game::Game(const std::string& gameName, const std::string& characterName, const std::string& currentLevel, const std::bitset<3u>& levels, std::size_t coins) :
	gameName(gameName),
	characterName(characterName),
	currentLevel(currentLevel),
	loadedLevels({ { "City", false }, { "Desert", false }, { "Forest", false } }),
	coins(coins)
{
	std::size_t i = 0;

	for (auto& level : loadedLevels)
	{
		level.second = levels[i];

		++i;
	}
}

std::string Game::getName() const
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

std::map<std::string, bool>& Game::getLoadedLevels()
{
	return this->loadedLevels;
}

std::size_t Game::getCoins() const
{
	return this->coins;
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

	for (auto& level : game.loadedLevels)
	{
		os << level.second;
	}

	return os;
}