/*
Copyright (c) 2017 InversePalindrome
Nihil - GameHud.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <Thor/Animations/Animator.hpp>
#include <Thor/Animations/FrameAnimation.hpp>


class  GameHud : public sf::Drawable, public sf::Transformable
{
public:
	GameHud(ResourceManager& resourceManager);

	void update(float deltaTime);

	std::size_t getNumberOfHitpoints() const;
	std::size_t getCoinDisplayNumber() const;

	void setNumberOfHitpoints(std::size_t numberOfHitpoints);
	void setCoinDisplayNumber(std::size_t coinDisplayNumber);

	void addToCoinDisplayNumber();

private:
	std::size_t numberOfHitpoints;
	std::size_t coinDisplayNumber;

	sf::Sprite heartSprite;
	sf::Sprite coinSprite;

	sf::Text coinText;

	thor::Animator<sf::Sprite, std::string> animator;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};