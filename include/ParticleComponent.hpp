/*
Copyright (c) 2017 InversePalindrome
Nihil - ParticleComponent.hpp
InversePalindrome.com
*/


#pragma once


#include "ParticleData.hpp"
#include "StateComponent.hpp"
#include "ResourceManager.hpp"

#include <Thor/Particles/Emitters.hpp>
#include <Thor/Particles/ParticleSystem.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <unordered_map>


class ParticleComponent : public sf::Drawable
{
public:
	ParticleComponent(const std::string& filePath, ResourceManager& resourceManager);

	void update(float deltaTime);
    
	ParticleData getParticleData(EntityState state) const;
	sf::Vector2f getEmissionOffset() const;

	void setEffect(EntityState state);
	void setEmitterPosition(const sf::Vector2f& position);

	void clearEffects();

	void clearParticles();

	bool hasParticleData(EntityState state) const;

private:
	thor::ParticleSystem particleSystem;
	thor::UniversalEmitter emitter;
	std::size_t rangeOfEmission;
	sf::Vector2f emissionOffset;

	std::unordered_map<EntityState, ParticleData> particleEffects;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};