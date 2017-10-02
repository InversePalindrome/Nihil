/*
Copyright (c) 2017 InversePalindrome
Nihil - ParticleComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "ParticleData.hpp"
#include "Component.hpp"
#include "StateComponent.hpp"
#include "ResourceManager.hpp"

#include <Thor/Particles/Emitters.hpp>
#include <Thor/Particles/ParticleSystem.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <unordered_map>


class ParticleComponent : public Component, public sf::Transformable, public sf::Drawable
{
	friend std::ostream& operator<<(std::ostream& os, const ParticleComponent& component);

public:
	ParticleComponent(const std::string& filePath, ResourceManager& resourceManager);

	void update(float deltaTime);
    
	ParticleData getParticleData(EntityState state) const;
	sf::Vector2f getEmissionOffset() const;
	sf::FloatRect getGlobalBounds() const;

	void setEffect(EntityState state);

	void clearEffects();

	void clearParticles();

	bool hasParticleData(EntityState state) const;

private:
	std::string pathFile;

	thor::ParticleSystem particleSystem;
	thor::UniversalEmitter emitter;
	float rangeOfEmission;
	sf::Vector2f emissionOffset;

	std::unordered_map<EntityState, ParticleData> particleEffects;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

std::ostream& operator<<(std::ostream& os, const ParticleComponent& component);