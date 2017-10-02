/*
Copyright (c) 2017 InversePalindrome
Nihil - TextComponent.cpp
InversePalindrome.com
*/


#include "TextComponent.hpp"
#include "TextStyleParser.hpp"


TextComponent::TextComponent(ResourceManager& resourceManager, const std::string& inputText, const std::string& pathFile) :
	Component("Text"),
	pathFile(pathFile)
{
	TextStyleParser::parseStyle(resourceManager, pathFile, text);

	text.setString(inputText);
    text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
}

std::ostream& operator<<(std::ostream& os, const TextComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.text.getString().toAnsiString() << ' ' << component.pathFile;;

	return os;
}

sf::FloatRect TextComponent::getGlobalBounds() const
{
	return this->text.getGlobalBounds();
}

void TextComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = this->getTransform();

	target.draw(this->text, states);
}