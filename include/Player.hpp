/*
Copyright (c) 2017 InversePalindrome
Nihil - Player.hpp
InversePalindrome.com
*/


#pragma once

#include <cstddef>
#include <string>


class Player
{
public:
	Player(const std::string& pathFile);

	void saveData();

	std::string getCharacterName() const;
	std::size_t getCoins() const;
	std::size_t getCurrentLevel() const;

	void setCharacterName(const std::string& characterName);
	void setCoins(std::size_t coins);
	void setCurrentLevel(std::size_t currentLevel);

private:
	std::string pathFile;
	std::string characterName;
	std::size_t coins;
	std::size_t currentLevel;
};