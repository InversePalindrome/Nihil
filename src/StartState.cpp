/*
Copyright (c) 2017 InversePalindrome
Nihil - StartState.cpp
InversePalindrome.com
*/


#include "StartState.hpp"
#include "StateMachine.hpp"
#include "EffectParser.hpp"
#include "TextStyleParser.hpp"
#include "FilePaths.hpp"

#include <Thor/Math/Distributions.hpp>


StartState::StartState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	view(stateData.window.getDefaultView()),
	continueLabel("Press a key to Continue", stateData.resourceManager.getFont(FontsID::BITWONDER), 40u),
	emitter(Parsers::parseEmitter("StartEmitters.txt"))
{
	auto& backgroundTexture = stateData.resourceManager.getTexture(TexturesID::StartBackground);

	backgroundTexture.setRepeated(true);
	
	background.setTexture(backgroundTexture);

	Parsers::parseParticleSystem(stateData.resourceManager, "StartParticles.txt", particleSystem);
	particleSystem.addEmitter(thor::refEmitter(emitter));

	Parsers::parseStyle(stateData.resourceManager, "TitleStyle.txt", titleLabel);
	titleLabel.setString("Nihil");
	titleLabel.setOrigin(titleLabel.getGlobalBounds().width / 2.f, titleLabel.getGlobalBounds().height / 2.f);
	titleLabel.setPosition(this->view.getCenter());

	continueLabel.setOrigin(continueLabel.getGlobalBounds().width / 2.f, continueLabel.getGlobalBounds().height / 2.f);
	continueLabel.setPosition(sf::Vector2f(this->view.getCenter().x - 10.f, 1250.f));
	continueLabel.setFillColor(sf::Color::Black);

	stateData.soundManager.playMusic("StartElectro.wav", true);
}

void StartState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		this->transitionToMenu();
	}
}

void StartState::update(float deltaTime)
{
	this->stateData.guiManager.update(deltaTime);

	this->particleSystem.update(sf::seconds(deltaTime));

	this->view.move(this->viewSpeed * deltaTime, 0.f);

	this->background.setTextureRect(sf::IntRect(this->view.getCenter().x - this->view.getSize().x / 2u, 0u, this->view.getSize().x, this->view.getSize().y));
	this->background.setPosition(this->view.getCenter().x - this->view.getSize().x / 2.f, 0.f);

	this->emitter.setParticlePosition(thor::Distributions::rect(sf::Vector2f(this->view.getCenter().x -  this->view.getSize().x / 2.f - 200.f, 
		0.f), sf::Vector2f(this->view.getSize().x, this->view.getSize().y - 200.f)));
}

void StartState::draw()
{
	this->stateData.window.setView(this->view);

	this->stateData.window.draw(this->background);
	this->stateData.window.draw(this->particleSystem);

	this->stateData.window.setView(this->stateData.window.getDefaultView());

	this->stateData.window.draw(this->titleLabel);
	this->stateData.window.draw(this->continueLabel);
}

void StartState::transitionToMenu()
{
	this->stateData.soundManager.stopAllMusic();
	this->stateData.window.setView(this->stateData.window.getDefaultView());
	this->stateMachine.changeState(StateID::Menu);
}