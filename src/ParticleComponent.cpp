/*
Copyright (c) 2017 InversePalindrome
Nihil - ParticleComponent.cpp
InversePalindrome.com
*/


#include "ParticleComponent.hpp"
#include <Thor/Math/Distributions.hpp>

#include <fstream>
#include <sstream>


ParticleComponent::ParticleComponent(const std::string& pathFile, ResourceManager& resourceManager) :
	rangeOfEmission(0u)
{
	std::ifstream inFile(pathFile);
	std::string line;

	std::size_t numberOfParticles = 0u;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string category;

		iStream >> category;

		if (category == "Texture")
		{
			std::size_t textureID = 0u, radiusOfEmission = 0u;
			float emissionOffsetX = 0.f, emissionOffsetY = 0.f;

			iStream >> textureID >> radiusOfEmission >> emissionOffsetX >> emissionOffsetY;

			particleSystem.setTexture(resourceManager.getTexture(static_cast<TexturesID>(textureID)));

			rangeOfEmission = radiusOfEmission;
			emissionOffset = sf::Vector2f(emissionOffsetX, emissionOffsetY);
		}
		else if (category == "Particle")
		{
			std::size_t left = 0u, top = 0u, width = 0u, height = 0u;

			iStream >> left >> top >> width >> height;

			particleSystem.addTextureRect(sf::IntRect(left, top, width, height));

			++numberOfParticles;
		}
		else if (category == "State")
		{
			std::size_t stateID = 0u, R = 0u, G = 0u, B = 0u;
			float particleLifetime = 0.f, emissionRate = 0.f;

			iStream >> stateID >> emissionRate >> particleLifetime >> R >> G >> B;

			particleEffects[static_cast<EntityState>(stateID)] = ParticleData(emissionRate, particleLifetime, sf::Color(R, G, B));
		}
	}

	emitter.setParticleTextureIndex(thor::Distributions::uniform(0u, numberOfParticles - 1u));

	particleSystem.addEmitter(thor::refEmitter(emitter));
}

void ParticleComponent::update(float deltaTime)
{
	this->particleSystem.update(sf::seconds(deltaTime));
}

ParticleData ParticleComponent::getParticleData(EntityState state) const
{
	return this->particleEffects.at(state);
}

sf::Vector2f ParticleComponent::getEmissionOffset() const
{
	return this->emissionOffset;
}

void ParticleComponent::setEffect(EntityState state)
{
	this->emitter.setParticleLifetime(sf::seconds(this->particleEffects.at(state).particleLifetime));
	this->emitter.setEmissionRate(this->particleEffects.at(state).emissionRate);
	this->emitter.setParticleColor(this->particleEffects.at(state).particleColor);
}

void ParticleComponent::setEmitterPosition(const sf::Vector2f& position)
{
	this->emitter.setParticlePosition(thor::Distributions::circle(position, this->rangeOfEmission));
}

void ParticleComponent::clearEffects()
{
	this->emitter.setParticleLifetime(sf::Time::Zero);
	this->emitter.setEmissionRate(0.f);
}

void ParticleComponent::clearParticles()
{
	this->particleSystem.clearParticles();
}

bool ParticleComponent::hasParticleData(EntityState state) const
{
	return this->particleEffects.count(state);
}

void ParticleComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->particleSystem, states);
}