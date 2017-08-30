/*
Copyright (c) 2017 InversePalindrome
Nihil - Player.cpp
InversePalindrome.com
*/


#include "Player.hpp"

#include <fstream>


Player::Player(const std::string& pathFile) :
	pathFile(pathFile),
	currentLevel(),
	coins(0u)
{
	std::ifstream inFile(pathFile);
	
	inFile >> characterName >> currentLevel >> coins;
}

void Player::saveData()
{
	std::ofstream outFile(this->pathFile);

	outFile << this->characterName << " " << this->currentLevel << " " << this->coins;
}

std::string Player::getCharacterName() const
{
	return this->characterName;
}

std::string Player::getCurrentLevel() const
{
	return this->currentLevel;
}

std::size_t Player::getCoins() const
{
	return this->coins;
}

void Player::setCharacterName(const std::string& characterName)
{
	this->characterName = characterName;
}

void Player::setCurrentLevel(const std::string& currentLevel)
{
	this->currentLevel = currentLevel;
}

void Player::setCoins(std::size_t coins)
{
	this->coins = coins;
}