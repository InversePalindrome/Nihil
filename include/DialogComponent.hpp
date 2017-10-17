/*
Copyright (c) 2017 InversePalindrome
Nihil - DialogComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "TextComponent.hpp"
#include "SpriteComponent.hpp"
#include "ResourceManager.hpp"

#include <string>
#include <vector>


class DialogComponent : public Component, public sf::Transformable, public sf::Drawable
{
	friend std::ostream& operator<<(std::ostream& os, const DialogComponent& component);

public:
	DialogComponent(ResourceManager& resourceManager, const std::string& dialogFile, 
		const std::string& textStyleFile, const std::string& spriteFile, const sf::Vector2f& textOffset);

	void nextDialogue();

	sf::FloatRect getGlobalBounds() const;

private:
	std::string dialogue;
	std::string dialogFile;
	std::string textStyleFile;
	std::string spriteFile;
	std::vector<std::string> subDialogues;

	std::size_t currentSubDialog;

	TextComponent text;
	SpriteComponent sprite;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

std::ostream& operator<<(std::ostream& os, const DialogComponent& component);