/*
Copyright (c) 2017 InversePalindrome
Nihil - HealthBar.cpp
InversePalindrome.com
*/


#include "HealthBar.hpp"


HealthBar::HealthBar(ResourceManager& resourceManger) :
	heart()
{
	auto& heartTexture = resourceManger.getTexture(TexturesID::Heart);

	heartTexture.setRepeated(true);

	heart.setTexture(heartTexture);
	heart.setScale(0.65f, 0.65f);
}

void HealthBar::setHitpointsDisplay(std::size_t hitpoints)
{
	this->heart.setTextureRect(sf::IntRect(0.f, 0.f, this->heart.getTexture()->getSize().x * hitpoints, this->heart.getTexture()->getSize().y));
}

void HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (this->isVisible())
	{
		states.transform = this->getTransform();

		if (this->heart.getTextureRect().width > 0.f)
		{
			target.draw(this->heart, states);
		}
	}
}