/*
Copyright (c) 2017 InversePalindrome
Nihil - StartState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <Thor/Particles/Emitters.hpp>
#include <Thor/Particles/ParticleSystem.hpp>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>


class StartState : public State
{
public:
	StartState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
	static constexpr float viewSpeed = -200.f;

	sf::Sprite background;
	sf::View view;

	sf::Text titleLabel;
	sf::Text continueLabel;

	thor::ParticleSystem particleSystem;
	thor::UniversalEmitter emitter;

	void transitionToMenu();
};