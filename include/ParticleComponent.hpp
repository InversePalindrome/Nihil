/*
Copyright (c) 2017 InversePalindrome
Nihil - ParticleComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Renderable.hpp"
#include "Component.hpp"
#include "StateComponent.hpp"
#include "ResourceManager.hpp"

#include <Thor/Particles/Emitters.hpp>
#include <Thor/Particles/ParticleSystem.hpp>

#include <unordered_map>


class ParticleComponent : public Component, public Renderable
{
	friend std::ostream& operator<<(std::ostream& os, const ParticleComponent& component);

public:
	ParticleComponent(ResourceManager& resourceManager, const sf::Vector2f& effectRange, 
		const std::string& particleFile, const std::string& emitterFile);

	void update(float deltaTime);
    
	sf::FloatRect getGlobalBounds() const;

private:
	sf::Vector2f effectRange;
	std::string particleFile;
	std::string emitterFile;

	thor::ParticleSystem particleSystem;
	thor::UniversalEmitter emitter;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

std::ostream& operator<<(std::ostream& os, const ParticleComponent& component);