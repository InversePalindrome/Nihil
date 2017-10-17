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


DialogComponent::DialogComponent(ResourceManager& resourceManager, const std::string& dialogFile, 
	const std::string& textStyleFile, const std::string& spriteFile, const sf::Vector2f& textOffset) :
	Component("Dialog"),
	dialogFile(dialogFile),
	textStyleFile(textStyleFile),
	spriteFile(spriteFile),
	currentSubDialog(0u),
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

	if (!subDialogues.empty())
	{
		text.setText(subDialogues.front());
	}
}

std::ostream& operator<<(std::ostream& os, const DialogComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' <<
		component.dialogFile << ' ' << component.textStyleFile << ' ' << component.spriteFile;

	return os;
}

void DialogComponent::nextDialogue()
{
	if (this->currentSubDialog < this->subDialogues.size() - 1)
	{
		++this->currentSubDialog;

		text.setText(this->subDialogues[this->currentSubDialog]);
	}
	else
	{
		this->currentSubDialog = 0u;
	}
}

sf::FloatRect DialogComponent::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

void DialogComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = this->getTransform();

	target.draw(sprite, states);
	target.draw(text, states);
}