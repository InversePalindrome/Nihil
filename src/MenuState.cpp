/*
Copyright (c) 2017 InversePalindrome
Nihil - MenuState.cpp
InversePalindrome.com
*/


#include "MenuState.hpp"
#include "StateMachine.hpp"

#include <Thor/Math/Distributions.hpp>
#include <Thor/Particles/Emitters.hpp>
#include <Thor/Particles/Affectors.hpp>
#include <Thor/Graphics/ColorGradient.hpp>
#include <Thor/Animations/FadeAnimation.hpp>
#include <Thor/Animations/ColorAnimation.hpp>


MenuState::MenuState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	background(stateData.resourceManager.getTexture(TexturesID::MenuBackground)),
	titleLabel("Nihil", stateData.resourceManager.getFont(FontsID::BITWONDER), 220u),
	playButton(sfg::Button::Create("\t\t\tPlay\t\t\t")),
	settingsButton(sfg::Button::Create("   Settings  ")),
	charactersButton(sfg::Button::Create("Characters"))
{
	stateData.window.setView(stateData.window.getDefaultView());

	background.setScale(stateData.window.getSize().x / background.getGlobalBounds().width, stateData.window.getSize().y / background.getGlobalBounds().height);

	particleSystem.setTexture(stateData.resourceManager.getTexture(TexturesID::Particles));
	particleSystem.addTextureRect(sf::IntRect(0u, 0u, 400u, 400u));
	particleSystem.addTextureRect(sf::IntRect(400u, 0u, 400u, 400u));
	particleSystem.addTextureRect(sf::IntRect(0u, 400u, 400u, 400u));
	particleSystem.addTextureRect(sf::IntRect(656u, 528u, 128u, 128u));

	thor::ColorGradient colors;
	colors[0.f] = sf::Color(44, 221, 55);
	colors[0.25f] = sf::Color(47, 47, 204);
	colors[0.5f] = sf::Color(153, 47, 143);
	colors[0.75f] = sf::Color(241, 148, 55);
	colors[1.f] = sf::Color(255, 247, 4);

	thor::UniversalEmitter emitter;

	emitter.setParticleTextureIndex(thor::Distributions::uniform(0u, 2u));
	emitter.setEmissionRate(thor::Distributions::uniform(2.f, 5.f)());
	emitter.setParticlePosition(thor::Distributions::rect(sf::Vector2f(stateData.window.getSize().x / 2.f, stateData.window.getSize().y / 2.f),
		sf::Vector2f(stateData.window.getSize().x / 2.f, stateData.window.getSize().y / 2.f)));
	emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(3.f), sf::seconds(9.f)));
	emitter.setParticleRotation(thor::Distributions::uniform(0.f, 360.f));

	particleSystem.addEmitter(emitter);
	particleSystem.addAffector(thor::AnimationAffector(thor::ColorAnimation(colors)));
	particleSystem.addAffector(thor::AnimationAffector(thor::FadeAnimation(0.2f, 0.2f)));

	titleLabel.setPosition(sf::Vector2f(600u, 300.f));
	titleLabel.setFillColor(sf::Color(156u, 28u, 107u));
	titleLabel.setOutlineThickness(10.f);
	titleLabel.setOutlineColor(sf::Color(75u, 0u, 130u));

	playButton->SetPosition(sf::Vector2f(790.f, 820.f));
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToPlay(); });
	
	settingsButton->SetPosition(sf::Vector2f(790.f, 1000.f));
	settingsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToSettings(); });
	
	charactersButton->SetPosition(sf::Vector2f(790.f, 1180.f));
	charactersButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToCharacters(); });
	
	stateData.guiManager.setProperty("*", "FontName", "Resources/Fonts/8-BIT-WONDER.ttf");
	stateData.guiManager.setProperty("*", "Color", sf::Color(255u, 255u, 0u));
	stateData.guiManager.setProperty("*", "FontSize", 40.f);
	stateData.guiManager.setProperty("Button", "Padding", 25.f);
	stateData.guiManager.setProperty("Button", "BackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Button:PRELIGHT", "BackgroundColor", sf::Color(156u, 28u, 107u));
	stateData.guiManager.setProperty("Button:ACTIVE", "BackgroundColor", sf::Color(75u, 0u, 130u));
	stateData.guiManager.setProperty("Button:PRELIGHT", "Color", sf::Color(255u, 255u, 0u));
	stateData.guiManager.setProperty("Button:ACTIVE", "Color", sf::Color(255u, 255u, 0u));

	stateData.guiManager.addWidget(playButton);
	stateData.guiManager.addWidget(settingsButton);
	stateData.guiManager.addWidget(charactersButton);

	stateData.soundManager.playMusic("MenuDisco.wav", true);
}

void MenuState::handleEvent(const sf::Event& event)
{

}

void MenuState::update(float deltaTime)
{
	this->particleSystem.update(sf::seconds(deltaTime));
}

void MenuState::draw()
{
	this->stateData.window.draw(this->background);
	this->stateData.window.draw(this->particleSystem);
	this->stateData.window.draw(this->titleLabel);

	this->playButton->Show(true);
	this->settingsButton->Show(true);
	this->charactersButton->Show(true);
}

void MenuState::transitionToPlay()
{
	this->stateMachine.pushState(StateID::Hub);
}

void MenuState::transitionToSettings()
{
	this->stateMachine.pushState(StateID::Settings);
}

void MenuState::transitionToCharacters()
{
	this->stateMachine.pushState(StateID::Characters);
}