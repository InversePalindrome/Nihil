/*
Copyright (c) 2017 InversePalindrome
Nihil - CoinDisplay.cpp
InversePalindrome.com
*/


#include "CoinDisplay.hpp"
#include "AnimationParser.hpp"
#include "TextStyleParser.hpp"

#include <Thor/Animations/FrameAnimation.hpp>


CoinDisplay::CoinDisplay(ResourceManager& resourceManager) :
	coin(resourceManager.getTexture(TexturesID::Coin)),
	text(std::to_string(0), resourceManager.getFont(FontsID::BITWONDER), 75u)
{
	coin.setScale(1.5f, 1.5f);

	Parsers::parseStyle(resourceManager, "CoinStyle.txt", text);

	text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
	text.setPosition(155.f, 27.f);

	thor::FrameAnimation frameAnimation;
	std::size_t animationID = 0u;
	float animationTime = 0.f;

	Parsers::parseFrameAnimations("CoinAnimations.txt", frameAnimation, animationID, animationTime);

	animator.addAnimation("Animation", frameAnimation, sf::seconds(animationTime));

	animator.playAnimation("Animation", true);
}

void CoinDisplay::update(float deltaTime)
{
	this->animator.animate(this->coin);
	this->animator.update(sf::seconds(deltaTime));
}

void CoinDisplay::setNumberOfCoins(std::size_t coins)
{
	this->text.setString(std::to_string(coins));
}

void CoinDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (this->isVisible())
	{
		states.transform *= this->getTransform();

		target.draw(this->coin, states);
		target.draw(this->text, states);
	}
}