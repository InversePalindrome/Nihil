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


DialogComponent::DialogComponent(ResourceManager& resourceManager, float dialogueTime, const std::string& dialogFile, 
	const std::string& textStyleFile, const std::string& spriteFile, const sf::Vector2f& textOffset, const sf::Vector2f& positionOffset) :
	Component("Dialog"),
	dialogueTime(dialogueTime),
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
	setOffset(positionOffset);
}

std::ostream& operator<<(std::ostream& os, const DialogComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' '  << component.dialogueTime << ' ' 
		<< component.dialogFile << ' ' << component.textStyleFile << ' ' << component.spriteFile << ' ' 
		<< component.textOffset.x << ' ' << component.textOffset.y << ' ' << component.getOffset().x << ' ' << component.getOffset().y;

	return os;
}

void DialogComponent::nextDialogue()
{
	if (!this->subDialogues.empty() && this->dialogueCount < this->subDialogues.size())
	{
		this->text.setText(this->subDialogues[this->dialogueCount]);

		++this->dialogueCount;
	}
	else
	{
		this->dialogueCount = 0u;
		this->visibilityStatus = false;
	}
}

float DialogComponent::getDialogueTime() const
{
	return this->dialogueTime;
}

sf::FloatRect DialogComponent::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

void DialogComponent::setVisibilityStatus(bool visibilityStatus)
{
	this->visibilityStatus = visibilityStatus;
	this->dialogueCount = 0u;
}

void DialogComponent::setDialogueTime(float dialogueTime)
{
	this->dialogueTime = dialogueTime;
}

bool DialogComponent::isVisible() const
{
	return this->visibilityStatus;
}

bool DialogComponent::hasDialogueFinished() const
{
	return this->dialogueCount == this->subDialogues.size();
}

void DialogComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (this->visibilityStatus)
	{
		states.transform *= this->getTransform();

		target.draw(sprite, states);
		target.draw(text, states);
	}
}