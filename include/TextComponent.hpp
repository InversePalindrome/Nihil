/*
Copyright (c) 2017 InversePalindrome
Nihil - TextComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Renderable.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/Text.hpp>


class TextComponent : public Component, public Renderable
{
	friend std::ostream& operator<<(std::ostream& os, const TextComponent& component);

public:
	TextComponent(ResourceManager& resourceManager, const std::string& inputText, const std::string& fileName);

	sf::FloatRect getGlobalBounds() const;

	void setText(const std::string& text);

private:
	sf::Text text;
	std::string fileName;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

std::ostream& operator<<(std::ostream& os, const TextComponent& component);