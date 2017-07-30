/*
Copyright (c) 2017 InversePalindrome
Nihil - StartState.cpp
InversePalindrome.com
*/


#include "StartState.hpp"
#include "StateMachine.hpp"

#include <Thor/Math/Distributions.hpp>
#include <Thor/Particles/Affectors.hpp>
#include <Thor/Graphics/ColorGradient.hpp>
#include <Thor/Animations/FadeAnimation.hpp>
#include <Thor/Animations/ColorAnimation.hpp>


StartState::StartState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	view(stateData.window.getDefaultView()),
	titleLabel("Nihil", stateData.resourceManager.getFont(FontsID::BITWONDER), 220u),
	continueLabel("Press a key to Continue", stateData.resourceManager.getFont(FontsID::BITWONDER), 40u)
{
	auto& backgroundTexture = stateData.resourceManager.getTexture(TexturesID::StartBackground);

	backgroundTexture.setRepeated(true);
	
	background.setTexture(backgroundTexture);

	particleSystem.setTexture(stateData.resourceManager.getTexture(TexturesID::Particles));
	particleSystem.addTextureRect(sf::IntRect(0u, 0u, 400u, 400u));
	particleSystem.addTextureRect(sf::IntRect(0u, 400u, 100u, 96u));
	particleSystem.addTextureRect(sf::IntRect(400u, 0u, 256u, 256u));

	thor::ColorGradient colors;
	colors[0.f] = sf::Color(44, 221, 55);
	colors[0.25f] = sf::Color(47, 47, 204);
	colors[0.5f] = sf::Color(153, 47, 143);
	colors[0.75f] = sf::Color(241, 148, 55);
	colors[1.f] = sf::Color(255, 247, 4);

	emitter.setParticleTextureIndex(thor::Distributions::uniform(0u, 2u));
	emitter.setEmissionRate(thor::Distributions::uniform(8.f, 13.f)());
	emitter.setParticlePosition(thor::Distributions::rect(sf::Vector2f(stateData.window.getSize().x / 2.f, stateData.window.getSize().y / 2.f),
		sf::Vector2f(stateData.window.getSize().x / 2.f, stateData.window.getSize().y / 2.f)));
	emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(5.f), sf::seconds(10.f)));
	emitter.setParticleRotation(thor::Distributions::uniform(0.f, 360.f));

	particleSystem.addEmitter(thor::refEmitter(emitter));
	particleSystem.addAffector(thor::AnimationAffector(thor::ColorAnimation(colors)));
	particleSystem.addAffector(thor::AnimationAffector(thor::FadeAnimation(0.2f, 0.2f)));
	particleSystem.addAffector(thor::ForceAffector(sf::Vector2f(0.f, 20.f)));

	titleLabel.setOrigin(titleLabel.getGlobalBounds().width / 2.f, titleLabel.getGlobalBounds().height / 2.f);
	titleLabel.setPosition(this->view.getCenter());
	
	continueLabel.setOrigin(continueLabel.getGlobalBounds().width / 2.f, continueLabel.getGlobalBounds().height / 2.f);
	continueLabel.setPosition(sf::Vector2f(this->view.getCenter().x, 1250.f));

	continueLabel.setFillColor(sf::Color::Black);
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

	this->background.setTextureRect(sf::IntRect(this->view.getCenter().x - this->view.getSize().x / 2.f, 0.f, this->view.getSize().x, this->view.getSize().y));
	this->background.setPosition(this->view.getCenter().x - this->view.getSize().x / 2.f, 0.f);

	this->emitter.setParticlePosition(thor::Distributions::rect(sf::Vector2f(this->view.getCenter().x -  this->view.getSize().x / 2.f - 200.f, 
		0.f), sf::Vector2f(this->view.getSize().x, this->view.getSize().y - 200.f)));
	
	this->titleLabel.setPosition(this->view.getCenter());
	continueLabel.setPosition(sf::Vector2f(this->view.getCenter().x, 1250.f));

	this->stateData.window.setView(view);
}

void StartState::draw()
{
	this->stateData.window.draw(this->background);
	this->stateData.window.draw(this->particleSystem);
	this->stateData.window.draw(this->titleLabel);
	this->stateData.window.draw(this->continueLabel);
}

void StartState::transitionToMenu()
{
	this->stateMachine.changeState(StateID::Menu);
}