/*
Copyright (c) 2017 InversePalindrome
Nihil - SettingsState.cpp
InversePalindrome.com
*/


#include "SettingsState.hpp"
#include "StateMachine.hpp"
#include "GUIParser.hpp"
#include "FilePaths.hpp"

#include <SFGUI/RadioButton.hpp>

#include <sstream>
#include <fstream>


SettingsState::SettingsState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	backButton(sfg::Button::Create("BACK")),
	soundLabel(sfg::Label::Create("Sound")),
	musicLabel(sfg::Label::Create("Music")),
	soundVolumeScale(sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL)),
	musicVolumeScale(sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL)),
	soundVolumeAdjustment(sfg::Adjustment::Create(stateData.soundManager.getSoundProperties().volume, 0.f, 105.f, 1.f, 10.f, 5.f)),
	musicVolumeAdjustment(sfg::Adjustment::Create(stateData.soundManager.getMusicProperties().volume, 0.f, 105.f, 1.f, 10.f, 5.f)),
	soundScrollbar(sfg::Scrollbar::Create()),
	musicScrollbar(sfg::Scrollbar::Create()),
	keyButtons(sfg::RadioButtonGroup::Create())
{
	backButton->SetPosition(sf::Vector2f(12.f, 25.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToState(); });

	soundLabel->SetPosition(sf::Vector2f(900.f, 475.f));

	soundScrollbar->SetRequisition(sf::Vector2f(1000.f, 50.f));
	soundScrollbar->SetPosition(sf::Vector2f(520.f, 550.f));
	soundScrollbar->SetAdjustment(soundVolumeAdjustment);
	
	soundVolumeAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect([this] { adjustSoundVolume(); });

	soundVolumeScale->SetAdjustment(soundVolumeAdjustment);

	musicLabel->SetPosition(sf::Vector2f(900.f, 725.f));

	musicScrollbar->SetRequisition(sf::Vector2f(1000.f, 50.f));
	musicScrollbar->SetPosition(sf::Vector2f(520.f, 800.f));
	musicScrollbar->SetAdjustment(musicVolumeAdjustment);

	musicVolumeScale->SetAdjustment(musicVolumeAdjustment);

	musicVolumeAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect([this] { adjustMusicVolume(); });

	auto moveRightButton = sfg::RadioButton::Create("Move Right", keyButtons);
	auto moveLeftButton = sfg::RadioButton::Create("Move Left", keyButtons);
	auto jumpButton = sfg::RadioButton::Create("Jump", keyButtons);
	auto shootButton = sfg::RadioButton::Create("Shoot", keyButtons);

	moveLeftButton->SetId("0");
	moveRightButton->SetId("1");
	jumpButton->SetId("2");
	shootButton->SetId("3");

	moveRightButton->SetPosition(sf::Vector2f(570.f, 1000.f));
	moveLeftButton->SetPosition(sf::Vector2f(1120.f, 1000.f));
	jumpButton->SetPosition(sf::Vector2f(570.f, 1100.f));
	shootButton->SetPosition(sf::Vector2f(1120.f, 1100.f));

	Parsers::parseGUIProperties(stateData.guiManager, "SettingsGUI.txt");

	stateData.guiManager.addWidget(backButton);
	stateData.guiManager.addWidget(soundLabel);
	stateData.guiManager.addWidget(musicLabel);
	stateData.guiManager.addWidget(soundScrollbar);
	stateData.guiManager.addWidget(musicScrollbar);
	stateData.guiManager.addWidget(moveRightButton);
	stateData.guiManager.addWidget(moveLeftButton);
	stateData.guiManager.addWidget(jumpButton);
	stateData.guiManager.addWidget(shootButton);
}

void SettingsState::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		this->changeKeyBinding(event.key.code);
		break;
	}
}

void SettingsState::update(float deltaTime)
{
	
}

void SettingsState::draw()
{

}

bool SettingsState::isTransparent() const
{
	return true;
}

void SettingsState::adjustSoundVolume()
{
	this->stateData.soundManager.setSoundVolume(this->soundVolumeScale->GetValue());
}

void SettingsState::adjustMusicVolume()
{
	this->stateData.soundManager.setMusicVolume(this->musicVolumeScale->GetValue());
}

void SettingsState::changeKeyBinding(sf::Keyboard::Key key)
{
	for (const auto& keyButton : this->keyButtons->GetMembers())
	{
		if (keyButton._Get()->IsActive())
		{
			this->stateData.inputHandler.changeKey(static_cast<Action>
				(std::stoull(keyButton._Get()->GetId())), key);
		}
	}
}

void SettingsState::saveSettings()
{
	std::ofstream outFile(Path::miscellaneous / "SettingsData.txt");

	this->stateData.soundManager.getSoundProperties().saveData("SoundData.txt");
	this->stateData.soundManager.getMusicProperties().saveData("MusicData.txt");
	this->stateData.inputHandler.saveData();
}

void SettingsState::transitionToState()
{
	this->saveSettings();

	this->stateMachine.popState();

	auto menu = this->stateMachine[this->stateMachine.size() - 2].get();

	menu->setVisibility(true);
	menu->showWidgets(true);
}