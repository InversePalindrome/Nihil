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
	DialogComponent(ResourceManager& resourceManager, std::size_t conversationID, const std::string& dialogFile, 
		const std::string& textStyleFile, const std::string& spriteFile, const sf::Vector2f& textOffset);

	void nextDialogue();

	void setVisibilityStatus(bool visibilityStatus);

	std::size_t getConversationID() const;
	sf::FloatRect getGlobalBounds() const;

	bool isVisible() const;

private:
	std::size_t conversationID;
	std::string dialogue;
	std::string dialogFile;
	std::string textStyleFile;
	std::string spriteFile;
	sf::Vector2f textOffset;
	std::vector<std::string> subDialogues;

	bool visibilityStatus;
	std::size_t dialogueCount;

	TextComponent text;
	SpriteComponent sprite;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

std::ostream& operator<<(std::ostream& os, const DialogComponent& component);