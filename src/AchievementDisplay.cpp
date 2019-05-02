/*
Copyright (c) 2017 InversePalindrome
Nihil - AchievementDisplay.cpp
InversePalindrome.com
*/


#include "AchievementDisplay.hpp"
#include "SpriteParser.hpp"
#include "TextStyleParser.hpp"


AchievementDisplay::AchievementDisplay(ResourceManager& resourceManager) :
    achievementNames({ {Achievement::Annihilator, "Annihilator"}, {Achievement::BigSpender, "BigSpender"},
    { Achievement::Collector, "Collector"}, {Achievement::Traveler, "Traveler"} })
{
    setVisibilityStatus(false);

    Parsers::parseSprite(resourceManager, "AchievementBackground.txt", background);

    Parsers::parseStyle(resourceManager, "AchievementText.txt", text);
    text.setPosition(118.f, 40.f);
}

void AchievementDisplay::displayAchievement(Achievement achievement)
{
    this->text.setString(this->achievementNames[achievement]);
    this->setVisibilityStatus(true);

    this->displayTimer.addCallbackTimer([this]() { this->setVisibilityStatus(false); }, 5.f);
}

void AchievementDisplay::update()
{
    this->displayTimer.update();
}

void AchievementDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (this->isVisible())
    {
        states.transform *= this->getTransform();

        target.draw(this->background, states);
        target.draw(this->text, states);
    }
}