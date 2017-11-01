/*
Copyright (c) 2017 InversePalindrome
Nihil - AchievementDisplay.hpp
InversePalindrome.com
*/


#pragma once

#include "Renderable.hpp"
#include "Achievement.hpp"
#include "ResourceManager.hpp"
#include "Callbacks.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <unordered_map>


class AchievementDisplay : public Renderable
{
public:
	AchievementDisplay(ResourceManager& resourceManager);

	void displayAchievement(Achievement achievement);

	void update();

private:
	sf::Text text;
	sf::Sprite background;

	std::unordered_map<Achievement, std::string> achievementNames;

	Callbacks displayTimer;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};