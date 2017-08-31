/*
Copyright (c) 2017 InversePalindrome
Nihil - Player.hpp
InversePalindrome.com
*/


#pragma once

#include <cstddef>
#include <string>

#include <SFML/System/Vector2.hpp>


class Player
{
public:
	Player(const std::string& pathFile);

	void saveData();

	std::string getCharacterName() const;
	std::string getCurrentLevel() const;

	std::size_t getCoins() const;
	sf::Vector2f getPosition() const;

	void setCharacterName(const std::string& characterName);
	void setCurrentLevel(const std::string& currentLevel);

	void setCoins(std::size_t coins);
	void setPosition(const sf::Vector2f& position);

private:
	std::string pathFile;
	std::string characterName;
	std::string currentLevel;

	std::size_t coins;
	sf::Vector2f position;
};