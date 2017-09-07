/*
Copyright (c) 2017 InversePalindrome
Nihil - Game.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/System/Vector2.hpp>

#include <cstddef>
#include <string>
#include <ostream>
#include <map>
#include <bitset>


class Game
{
	friend std::ostream& operator<<(std::ostream& os, const Game& game);

public:
	Game(const std::string& gameName);
	Game(const std::string& gameName, const std::string& characterName, const std::string& currentLevel, const std::bitset<3u>& levels, std::size_t coins);

	std::string getName() const;
	std::string getCharacterName() const;
	std::string getCurrentLevel() const;
	std::map<std::string, bool>& getLoadedLevels();

	std::size_t getCoins() const;

	void setCharacterName(const std::string& characterName);
	void setCurrentLevel(const std::string& currentLevel);

	void setCoins(std::size_t coins);

private:
	std::string gameName;
	std::string characterName;
	std::string currentLevel;
	std::map<std::string, bool> loadedLevels;

	std::size_t coins;
};

std::ostream& operator<<(std::ostream& os, const Game& game);