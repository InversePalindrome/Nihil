/*
Copyright (c) 2017 InversePalindrome
Nihil - TextComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>


class TextComponent : public Component, public sf::Transformable, public sf::Drawable
{
	friend std::ostream& operator<<(std::ostream& os, const TextComponent& component);

public:
	TextComponent(ResourceManager& resourceManager, const std::string& inputText, const std::string& pathFile);

	sf::FloatRect getGlobalBounds() const;

private:
	sf::Text text;
	std::string pathFile;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

std::ostream& operator<<(std::ostream& os, const TextComponent& component);