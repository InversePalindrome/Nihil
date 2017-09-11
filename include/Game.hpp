/*
Copyright (c) 2017 InversePalindrome
Nihil - Game.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/System/Vector2.hpp>

#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include <cstddef>
#include <string>
#include <ostream>
#include <unordered_map>


class Game
{
	friend std::ostream& operator<<(std::ostream& os, const Game& game);

public:
	Game();
	Game(const std::string& data);

	std::string getGameName() const;
	std::string getCharacterName() const;
	std::string getCurrentLevel() const;
	std::size_t getCoins() const;

	std::unordered_map<std::string, bool>& getLevels();
	std::unordered_map<std::string, bool>& getCharacters();

	void setGameName(const std::string& name);
	void setCharacterName(const std::string& characterName);
	void setCurrentLevel(const std::string& currentLevel);
	void setCoins(std::size_t coins);

private:
	std::string gameName;
	std::string characterName;
	std::string currentLevel;
	std::size_t coins;
	std::unordered_map<std::string, bool> levels;
	std::unordered_map<std::string, bool> characters;

	void loadNames();
	void loadDataBitsets(std::unordered_map<std::string, bool>& dataset, const boost::dynamic_bitset<std::size_t>& bitset);
};

std::ostream& operator<<(std::ostream& os, const Game& game);