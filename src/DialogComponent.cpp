/*
Copyright (c) 2017 InversePalindrome
Nihil - DialogComponent.cpp
InversePalindrome.com
*/


#include "DialogComponent.hpp"
#include "FilePaths.hpp"

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <fstream>
#include <streambuf>


DialogComponent::DialogComponent(ResourceManager& resourceManager, std::size_t conversationID, const std::string& dialogFile, 
	const std::string& textStyleFile, const std::string& spriteFile, const sf::Vector2f& textOffset) :
	Component("Dialog"),
	conversationID(conversationID),
	dialogFile(dialogFile),
	textStyleFile(textStyleFile),
	spriteFile(spriteFile),
	textOffset(textOffset),
	visibilityStatus(false),
	dialogueCount(0u),
	text(resourceManager, "", textStyleFile),
	sprite(resourceManager, spriteFile)
{
	std::ifstream inFile(Path::dialogues / dialogFile);
	
	dialogue.assign((std::istreambuf_iterator<char>(inFile)),
		std::istreambuf_iterator<char>());

	boost::split(subDialogues, dialogue, boost::is_any_of("-"));

	for (auto& dialo : subDialogues)
	{
		boost::trim(dialo);
	}

	text.move(textOffset);
	setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
}

std::ostream& operator<<(std::ostream& os, const DialogComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.conversationID << ' ' << component.dialogFile 
		<< ' ' << component.textStyleFile << ' ' << component.spriteFile << ' ' << component.textOffset.x << ' ' << component.textOffset.y;

	return os;
}

void DialogComponent::nextDialogue()
{
	if (!this->subDialogues.empty() && this->dialogueCount < this->subDialogues.size())
	{
		this->text.setText(this->subDialogues.front());

		std::rotate(std::begin(this->subDialogues), std::begin(this->subDialogues) + 1, std::end(this->subDialogues));

		++this->dialogueCount;
	}
	else
	{
		this->dialogueCount = 0u;
		this->visibilityStatus = false;
	}
}

void DialogComponent::setVisibilityStatus(bool visibilityStatus)
{
	this->visibilityStatus = visibilityStatus;
}

std::size_t DialogComponent::getConversationID() const
{
	return this->conversationID;
}

sf::FloatRect DialogComponent::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

bool DialogComponent::isVisible() const
{
	return this->visibilityStatus;
}

void DialogComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (this->visibilityStatus)
	{
		states.transform = this->getTransform();

		target.draw(sprite, states);
		target.draw(text, states);
	}
}