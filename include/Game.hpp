/*
Copyright (c) 2017 InversePalindrome
Nihil - Game.hpp
InversePalindrome.com
*/


#pragma once

#include <SFML/System/Vector2.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include <cstddef>
#include <string>
#include <ostream>


struct LoadedLevelData
{
	std::string name;
	sf::Vector2f spawnPosition;
	bool isLoaded;
};

struct LoadedCharacterData
{
	std::string name;
	bool isLoaded;
};

class Game
{
	friend std::ostream& operator<<(std::ostream& os, const Game& game);

	using LoadedLevels = boost::multi_index_container<LoadedLevelData,
		boost::multi_index::indexed_by<boost::multi_index::random_access<>,
		boost::multi_index::hashed_unique<boost::multi_index::member<LoadedLevelData, std::string, &LoadedLevelData::name>>>>;

	using LoadedCharacters = boost::multi_index_container<LoadedCharacterData,
		boost::multi_index::indexed_by<boost::multi_index::random_access<>,
		boost::multi_index::hashed_unique<boost::multi_index::member<LoadedCharacterData, std::string, &LoadedCharacterData::name>>>>;

public:
	Game();
	Game(const std::string& data);

	std::string getGameName() const;
	std::string getCharacterName() const;
	std::string getCurrentLevel() const;
	std::size_t getCoins() const;
	sf::Vector2f getSpawnPoint() const;

	LoadedLevels& getLevels();
	LoadedCharacters& getCharacters();

	void setGameName(const std::string& name);
	void setCharacterName(const std::string& characterName);
	void setCurrentLevel(const std::string& currentLevel);
	void setCoins(std::size_t coins);

private:
	std::string gameName;
	std::string characterName;
	std::string currentLevel;
	std::size_t coins;

	LoadedLevels levels;
	LoadedCharacters characters;

	void loadNames();
	void loadSpawnPoints();

	template<typename T>
	void loadDataBitsets(T& dataset, const boost::dynamic_bitset<std::size_t>& bitset);
};

std::ostream& operator<<(std::ostream& os, const Game& game);

template<typename T>
void Game::loadDataBitsets(T& dataset, const boost::dynamic_bitset<std::size_t>& bitset)
{
	std::size_t i = bitset.size() - 1u;

	for (auto data = std::begin(dataset); data != std::end(dataset); ++data)
	{
		dataset.modify(data, [&bitset, i](auto& iData) { iData.isLoaded = bitset[i]; });

		--i;
	}
}