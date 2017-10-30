/*
Copyright (c) 2017 InversePalindrome
Nihil - Game.cpp
InversePalindrome.com
*/


#include "Game.hpp"
#include "FilePaths.hpp"

#include <fstream>
#include <sstream>


Game::Game() 
{
	loadLevels();
	loadAchievements();

	std::ifstream inFile(Path::levels / "Levels.txt");

	inFile >> currentLevel;
}

Game::Game(const std::string& data) 
{
	std::istringstream iStream(data);
	std::string line;

	std::string levelBits;

	while (std::getline(iStream, line))
	{
		std::istringstream inLine(line);

		std::string category;

		inLine >> category;

		if (category == "Game")
		{
			inLine >> gameName;
		}
		else if (category == "CurrentLevel")
		{
			inLine >> currentLevel;
		}
		else if (category == "Levels")
		{
			inLine >> levelBits;
		}
		else if (category == "Item")
		{
			std::size_t item = 0u, quantity = 0u;

			inLine >> item >> quantity;

			this->items.emplace(static_cast<Item>(item), quantity);
		}
		else if (category == "Achievement")
		{
			std::size_t achievementID = 0u, currentQuantity = 0u, maxQuantity = 0u;

			inLine >> achievementID >> currentQuantity >> maxQuantity;

			this->achievements.emplace(static_cast<Achievement>(achievementID), std::make_pair(currentQuantity, maxQuantity));
		}
	}

	loadLevels();

    const auto& levelsBitset = boost::dynamic_bitset<std::size_t>(levelBits);

	loadDataBitsets<LoadedLevels>(levels, levelsBitset);
}

std::optional<Entity> Game::getPlayer()
{
	if (this->player.get_status() != entityplus::entity_status::DELETED || 
		this->player.get_status() != entityplus::entity_status::UNINITIALIZED)
	{
		this->player.sync();

		return this->player;
	}
	else
	{
		return {};
	}
}

std::string Game::getGameName() const
{
	return this->gameName;
}

std::string Game::getCurrentLevel() const
{
	return this->currentLevel;
}

Items& Game::getItems() 
{
	return this->items;
}

Achievements& Game::getAchievements()
{
	return this->achievements;
}

sf::Vector2f Game::getCurrentSpawnPoint() const
{
	return this->levels.get<1>().find(this->currentLevel)->spawnPosition;
}

Game::LoadedLevels& Game::getLevels()
{
	return this->levels;
}

DirectionType Game::getCurrenDirectionType() const
{
	return this->levels.get<1>().find(this->currentLevel)->directionType;
}

b2Vec2 Game::getCurrentGravity() const
{
	return this->levels.get<1>().find(this->currentLevel)->gravity;
}

void Game::setPlayer(Entity player)
{
	this->player = player;
}

void Game::setGameName(const std::string& gameName)
{
	this->gameName = gameName;
}

void Game::setCurrentLevel(const std::string& currentLevel)
{
	this->currentLevel = currentLevel;
}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
	os << "Game " << game.gameName << '\n';
	os << "CurrentLevel " << game.currentLevel << '\n';

	os << "Levels ";

	for (const auto& level : game.levels)
	{
		os << level.isLoaded;
	}
	
	os << '\n';

	for (const auto& item : game.items)
	{
		os << "Item " << static_cast<std::size_t>(item.first) << ' ' << item.second << '\n';
	}

	for (const auto& achievement : game.achievements)
	{
		os << "Achievement" << ' ' << static_cast<std::size_t>(achievement.first) << ' ' <<
			achievement.second.first << ' ' << achievement.second.second << '\n';
	}

	return os;
}

void Game::loadLevels()
{
	std::ifstream inFile(Path::levels / "Levels.txt");
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);
		
	    std::string levelName;
		std::size_t directionType = 0u;
		float xGravity = 0.f, yGravity = 0.f, xPosition = 0.f, yPosition = 0.f;

		iStream >> levelName >> directionType >> xGravity >> yGravity >> xPosition >> yPosition;

		this->levels.get<1>().insert({ levelName, static_cast<DirectionType>(directionType), { xGravity, yGravity }, {xPosition, yPosition}, false });
	}
}

void Game::loadAchievements()
{
	std::ifstream inFile(Path::miscellaneous / "AchievementsData.txt");
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::size_t achievementID = 0u, maxQuantity = 0u;

		iStream >> achievementID >> maxQuantity;

		this->achievements[static_cast<Achievement>(achievementID)] = { 0u, maxQuantity };
	}
}