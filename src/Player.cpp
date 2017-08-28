/*
Copyright (c) 2017 InversePalindrome
Nihil - Player.cpp
InversePalindrome.com
*/


#include "Player.hpp"

#include <fstream>


Player::Player(const std::string& pathFile) :
	pathFile(pathFile),
	coins(0u),
	currentLevel(0u)
{
	std::ifstream inFile(pathFile);
	
	inFile >> characterName >> coins >> currentLevel;
}

void Player::saveData()
{
	std::ofstream outFile(this->pathFile);

	outFile << this->characterName << " " << this->coins << " " << this->currentLevel;
}

std::string Player::getCharacterName() const
{
	return this->characterName;
}

std::size_t Player::getCoins() const
{
	return this->coins;
}

std::size_t Player::getCurrentLevel() const
{
	return this->currentLevel;
}

void Player::setCharacterName(const std::string& characterName)
{
	this->characterName = characterName;
}

void Player::setCoins(std::size_t coins)
{
	this->coins = coins;
}

void Player::setCurrentLevel(std::size_t currentLevel)
{
	this->currentLevel = currentLevel;
}