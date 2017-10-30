/*
Copyright (c) 2017 InversePalindrome
Nihil - AchievementsState.cpp
InversePalindrome.com
*/


#include "AchievementsState.hpp"
#include "StateMachine.hpp"
#include "FilePaths.hpp"
#include "SpriteParser.hpp"
#include "GUIParser.hpp"
#include "TextStyleParser.hpp"

#include <SFGUI/Scale.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Image.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Adjustment.hpp>

#include <boost/algorithm/string/replace.hpp>

#include <fstream>


AchievementsState::AchievementsState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	backButton(sfg::Button::Create("Back")),
	scrolledWindow(sfg::ScrolledWindow::Create()),
	achievementIDs({ {Achievement::Annihilator, "Annihilator" }, {Achievement::Traveler, "Traveller"},
	{Achievement::Collector, "Collector"}, {Achievement::BigSpender, "BigSpender"} })
{
	Parsers::parseSprite(stateData.resourceManager, "AchievementsPanel.txt", background);
	background.setOrigin(background.getGlobalBounds().width / 2.f, background.getGlobalBounds().height / 2.f);
	background.setPosition(810.f, 755.f);

	Parsers::parseSprite(stateData.resourceManager, "MediumTitleBar.txt", titleBar);
	titleBar.setOrigin(titleBar.getLocalBounds().width / 2.f, titleBar.getLocalBounds().height / 2.f);
	titleBar.setPosition(stateData.window.getDefaultView().getCenter().x - 24.f, 305.f);

	Parsers::parseStyle(stateData.resourceManager, "StateTitle.txt", title);
	title.setString("Achievements");
	title.setOrigin(title.getGlobalBounds().width / 2.f, title.getGlobalBounds().height / 2.f);
	title.setPosition(stateData.window.getDefaultView().getCenter().x - 14.f, 299.f);

	backButton->SetPosition(sf::Vector2f(12.f, 25.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	auto scale = sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL);
	auto adjustment = sfg::Adjustment::Create(0.f, 0.f, 100.f, 1.f, 10.f, 10.f);

	scrolledWindow->SetPosition({ 345.f, 450.f });
	scrolledWindow->SetRequisition(sf::Vector2f(1350.f, 890.f));
	scrolledWindow->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_NEVER);
	scrolledWindow->SetVerticalAdjustment(adjustment);

	scale->SetAdjustment(adjustment);

	stateData.guiManager.addWidget(backButton);
	stateData.guiManager.addWidget(scrolledWindow);

	Parsers::parseGUIProperties(stateData.guiManager, "AchievementsGUI.txt");

	loadAchievements("Achievements.txt");
}

void AchievementsState::handleEvent(const sf::Event& event)
{

}

void AchievementsState::update(float deltaTime)
{

}

void AchievementsState::draw()
{
	this->stateData.window.draw(this->background);
	this->stateData.window.draw(this->titleBar);
	this->stateData.window.draw(this->title);
}

bool AchievementsState::isTransparent() const
{
	return true;
}

bool AchievementsState::isDependent() const
{
	return true;
}

void AchievementsState::loadAchievements(const std::string& fileName)
{
	std::ifstream inFile(Path::miscellaneous / fileName);

	auto table = sfg::Table::Create();

	std::string description;
	std::size_t achievementID = 0u, imageID = 0u;

	std::size_t row = 0u, column = 0u;

	const auto& achievements = this->stateData.games.front().getAchievements();

	while (inFile >> achievementID >> description >> imageID)
	{
		boost::replace_all(description, "\\n", "\n");
		boost::replace_all(description, "_", " ");

		auto achievement = static_cast<Achievement>(achievementID);

		auto image = sfg::Image::Create(this->stateData.resourceManager.getImage(static_cast<ImagesID>(imageID)));

		auto info = sfg::Label::Create("\n\n" + achievementIDs[achievement] + "\n\n" + std::to_string(achievements.at(achievement).first)
			+ " / " + std::to_string(achievements.at(achievement).second));

		auto body = sfg::Label::Create("\n\n" + description);

		float yOffset = 50.f;

		table->Attach(image, { column, row, 1u, 1u }, 0u, 0u, { 40.f, yOffset });

		++column;

		table->Attach(info, { column, row, 1u, 1u }, 0u, 0u, { 80.f, yOffset });

		++column;
		
		table->Attach(body, { column, row, 2u, 1u }, 0u, 0u, { 0.f, yOffset });

		++row;
		column = 0u;
	}

	this->scrolledWindow->AddWithViewport(table);
}

void AchievementsState::transitionToMenu()
{
	this->stateMachine.popState();

	auto menu = this->stateMachine[this->stateMachine.size() - 2].get();

	menu->showWidgets(true);
}