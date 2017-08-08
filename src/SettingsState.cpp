/*
Copyright (c) 2017 InversePalindrome
Nihil - SettingsState.cpp
InversePalindrome.com
*/


#include "SettingsState.hpp"
#include "StateMachine.hpp"

#include <SFGUI/Image.hpp>


SettingsState::SettingsState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	backButton(sfg::Button::Create()),
	soundLabel(sfg::Label::Create("Sound")),
	musicLabel(sfg::Label::Create("Music")),
	soundVolumeScale(sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL)),
	musicVolumeScale(sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL)),
	soundVolumeAdjustment(sfg::Adjustment::Create(stateData.soundManager.getSoundProperties().volume, 0.f, 105.f, 1.f, 10.f, 5.f)),
	musicVolumeAdjustment(sfg::Adjustment::Create(stateData.soundManager.getMusicProperties().volume, 0.f, 105.f, 1.f, 10.f, 5.f)),
	soundScrollbar(sfg::Scrollbar::Create()),
	musicScrollbar(sfg::Scrollbar::Create()),
	keyButtons(sfg::RadioButtonGroup::Create()),
	moveRightButton(sfg::RadioButton::Create("Move Right", keyButtons)),
	moveLeftButton(sfg::RadioButton::Create("Move Left", keyButtons)),
	jumpButton(sfg::RadioButton::Create("Jump", keyButtons))
{
	backButton->SetImage(sfg::Image::Create(stateData.resourceManager.getImage(ImagesID::BackButton)));
	backButton->SetPosition(sf::Vector2f(12.f, 65.f));
	backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToMenu(); });

	soundLabel->SetPosition(sf::Vector2f(930.f, 475.f));

	soundScrollbar->SetRequisition(sf::Vector2f(1000.f, 50.f));
	soundScrollbar->SetPosition(sf::Vector2f(550.f, 550.f));
	soundScrollbar->SetAdjustment(soundVolumeAdjustment);

	soundVolumeAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect([this] { adjustSoundVolume(); });

	soundVolumeScale->SetAdjustment(soundVolumeAdjustment);

	musicLabel->SetPosition(sf::Vector2f(930.f, 725.f));

	musicScrollbar->SetRequisition(sf::Vector2f(1000.f, 50.f));
	musicScrollbar->SetPosition(sf::Vector2f(550.f, 800.f));
	musicScrollbar->SetAdjustment(musicVolumeAdjustment);

	musicVolumeScale->SetAdjustment(musicVolumeAdjustment);

	musicVolumeAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect([this] { adjustMusicVolume(); });

	moveRightButton->SetPosition(sf::Vector2f(600.f, 1000.f));
	moveLeftButton->SetPosition(sf::Vector2f(1150.f, 1000.f));
	jumpButton->SetPosition(sf::Vector2f(600.f, 1100.f));

	stateData.guiManager.setProperty("*", "FontName", "Resources/Fonts/8-BIT-WONDER.ttf");
	stateData.guiManager.setProperty("*", "Color", sf::Color(255u, 255u, 0u));
	stateData.guiManager.setProperty("*", "FontSize", 40.f);
	stateData.guiManager.setProperty("Scrollbar", "TroughColor", sf::Color(102u, 0u, 204u));
	stateData.guiManager.setProperty("Scrollbar", "SliderColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("Scrollbar", "BorderColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Scrollbar", "StepperBackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Scrollbar", "StepperArrowColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton", "BackgroundColor", sf::Color(102u, 0u, 204u));
	stateData.guiManager.setProperty("RadioButton", "BoxSize", 25.f);
	stateData.guiManager.setProperty("RadioButton:PRELIGHT", "BackgroundColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton:ACTIVE", "BackgroundColor", sf::Color(15u, 192u, 252u));
	stateData.guiManager.setProperty("RadioButton:SELECTED", "BackgroundColor", sf::Color(15u, 192u, 252u));

	stateData.guiManager.addWidget(backButton);
	stateData.guiManager.addWidget(soundLabel);
	stateData.guiManager.addWidget(musicLabel);
	stateData.guiManager.addWidget(soundScrollbar);
	stateData.guiManager.addWidget(musicScrollbar);
	stateData.guiManager.addWidget(moveRightButton);
	stateData.guiManager.addWidget(moveLeftButton);
	stateData.guiManager.addWidget(jumpButton);
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
	for (auto& keyButton : this->keyButtons->GetMembers())
	{
		if (keyButton._Get()->IsActive())
		{
			this->stateData.inputHandler.changeKey(keyButton._Get()->GetLabel(), thor::Action(key));
		}
	}
}

void SettingsState::transitionToMenu()
{
	this->stateMachine.popState();
}