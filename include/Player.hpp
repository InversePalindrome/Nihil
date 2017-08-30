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
	std::string getCurrentLevel() const;
	std::size_t getCoins() const;

	void setCharacterName(const std::string& characterName);
	void setCurrentLevel(const std::string& currentLevel);
	void setCoins(std::size_t coins);

private:
	std::string pathFile;
	std::string characterName;
	std::string currentLevel;
	std::size_t coins;
};