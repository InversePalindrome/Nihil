/*
Copyright (c) 2017 InversePalindrome
Nihil - TextComponent.cpp
InversePalindrome.com
*/


#include "TextComponent.hpp"
#include "TextStyleParser.hpp"


TextComponent::TextComponent(ResourceManager& resourceManager, const std::string& inputText, const std::string& fileName) :
	Component("Text"),
	fileName(fileName)
{
	Parsers::parseStyle(resourceManager, fileName, text);

	text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
	text.setString(inputText);
}

std::ostream& operator<<(std::ostream& os, const TextComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.text.getString().toAnsiString() << ' ' << component.fileName;;

	return os;
}

sf::FloatRect TextComponent::getGlobalBounds() const
{
	return this->text.getGlobalBounds();
}

void TextComponent::setText(const std::string& text)
{
	this->text.setString(text);
}

void TextComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(this->text, states);
}