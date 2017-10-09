/*
Copyright (c) 2017 InversePalindrome
Nihil - Game.hpp
InversePalindrome.com
*/


#pragma once

#include "Item.hpp"
#include "Direction.hpp"

#include <Box2D/Common/b2Math.h>
#include <SFML/System/Vector2.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include <cstddef>
#include <string>
#include <ostream>
#include <unordered_map>


struct LevelData
{
	std::string name;
	DirectionType directionType;
	b2Vec2 gravity;
	sf::Vector2f spawnPosition;
	bool isLoaded;
};

struct CharacterData
{
	std::string name;
	bool isLoaded;
};

class Game
{
	friend std::ostream& operator<<(std::ostream& os, const Game& game);

	using LoadedLevels = boost::multi_index_container<LevelData,
		boost::multi_index::indexed_by<boost::multi_index::random_access<>,
		boost::multi_index::hashed_unique<boost::multi_index::member<LevelData, std::string, &LevelData::name>>>>;

	using LoadedCharacters = boost::multi_index_container<CharacterData,
		boost::multi_index::indexed_by<boost::multi_index::random_access<>,
		boost::multi_index::hashed_unique<boost::multi_index::member<CharacterData, std::string, &CharacterData::name>>>>;

public:
	Game();
	Game(const std::string& data);

	std::string getGameName() const;
	std::string getCurrentCharacter() const;
	std::string getCurrentLevel() const;
	Items& getItems();
	DirectionType getCurrenDirectionType() const;
	b2Vec2 getCurrentGravity() const;
	sf::Vector2f getCurrentSpawnPoint() const;

	LoadedLevels& getLevels();
	LoadedCharacters& getCharacters();

	void setGameName(const std::string& name);
	void setCurrentCharacter(const std::string& currentCharacter);
	void setCurrentLevel(const std::string& currentLevel);

private:
	std::string gameName;
	std::string currentCharacter;
	std::string currentLevel;
	Items items; 

	LoadedLevels levels;
	LoadedCharacters characters;

	void loadCharacterNames();
	void loadLevels();

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