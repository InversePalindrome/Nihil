/*
Copyright (c) 2017 InversePalindrome
Nihil - CoinDisplay.hpp
InversePalindrome.com
*/


#pragma once

#include "ResourceManager.hpp"
#include "Renderable.hpp"
#include "Animation.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <Thor/Animations/Animator.hpp>

#include <string>


class CoinDisplay : public Renderable
{
public:
	CoinDisplay(ResourceManager& resourceManager);

	void update(float deltaTime);

	void setNumberOfCoins(std::size_t coins);

private:
	sf::Sprite coin;
	sf::Text text;

	Animator animator;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};