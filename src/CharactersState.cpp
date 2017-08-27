/*
Copyright (c) 2017 InversePalindrome
Nihil - CharactersState.cpp
InversePalindrome.com
*/


#include "CharactersState.hpp"
#include "StateMachine.hpp"

#include <SFGUI/Image.hpp>
#include <SFGUI/RadioButton.hpp>

#include <sstream>
#include <fstream>


CharactersState::CharactersState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	background(stateData.resourceManager.getTexture(TexturesID::MenuBackground)),
	backButton(sfg::Button::Create("BACK")),
	scrolledWindowBox(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 400.f)),
	scrollbarScale(sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL)),
	scrollbarAdjustment(sfg::Adjustment::Create(0.f, 0.f, 100.f, 1.f, 10.f, 10.f)),
	scrolledWindow(sfg::ScrolledWindow::Create()),
	characterButtons(sfg::RadioButtonGroup::Create())
{
	loadCharacters("Resources/Files/Characters.txt");

	background.setScale(stateData.window.getSize().x / background.getGlobalBounds().width, stateData.window.getSize().y / background.getGlobalBounds().height);

	backButton->SetPosition(sf::Vector2f(12.f, 65.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	scrolledWindowBox->SetPosition(sf::Vector2f(125.f, 320.f));
	scrolledWindowBox->SetRequisition(sf::Vector2f((characterButtons->GetMembers().size() + 0.75f) * scrolledWindowBox->GetSpacing(), 0.f));

	scrolledWindow->SetPosition(sf::Vector2f(125.f, 400.f));
	scrolledWindow->SetRequisition(sf::Vector2f(1800.f, 650.f));
	scrolledWindow->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_NEVER);
	
	scrolledWindow->SetHorizontalAdjustment(scrollbarAdjustment);
	
	scrolledWindow->AddWithViewport(scrolledWindowBox);
	
    scrollbarScale->SetAdjustment(scrollbarAdjustment);

	for (auto& characterButton : characterButtons->GetMembers())
	{
		characterButton._Get()->GetSignal(sfg::ToggleButton::OnToggle).Connect([this, &characterButton]() 
		{ 
			if (characterButton._Get()->IsActive())
			{
				selectedCharacter(characterButton._Get()->GetId());
			}
		});
	}

	stateData.guiManager.setProperty("ScrolledWindow", "BorderWidth", 0.f);
	stateData.guiManager.setProperty("ScrolledWindow", "ScrollbarSpacing", 20.f);
	stateData.guiManager.setProperty("Scrollbar", "TroughColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Scrollbar", "SliderColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("Scrollbar", "BorderColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Scrollbar", "StepperBackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Scrollbar", "StepperArrowColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton", "BackgroundColor", sf::Color(102u, 0u, 204u));
	stateData.guiManager.setProperty("RadioButton", "BoxSize", 30.f);
	stateData.guiManager.setProperty("RadioButton:PRELIGHT", "BackgroundColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton:ACTIVE", "BackgroundColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton:SELECTED", "BackgroundColor", sf::Color(15u, 192u, 252u));

	stateData.guiManager.addWidget(backButton);
	stateData.guiManager.addWidget(scrolledWindow);
}

void CharactersState::handleEvent(const sf::Event& event)
{

}

void CharactersState::update(float deltaTime)
{

}

void CharactersState::draw()
{
	this->stateData.window.draw(this->background);
}

void CharactersState::loadCharacters(const std::string& filePath)
{
	std::ifstream inFile(filePath);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string characterID;
		std::size_t imageID;

		iStream >> characterID >> imageID;

		auto characterButton = sfg::RadioButton::Create("", this->characterButtons);

		characterButton->SetId(characterID);
		characterButton->SetImage(sfg::Image::Create(this->stateData.resourceManager.getImage(static_cast<ImagesID>(imageID))));
		characterButton->GetImage()->UpdateDrawablePosition();

		this->scrolledWindowBox->Pack(characterButton);
	}
}

void CharactersState::selectedCharacter(const std::string& character)
{
	
}

void CharactersState::transitionToMenu()
{
	this->stateMachine.popState();
}