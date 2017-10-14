/*
Copyright (c) 2017 InversePalindrome
Nihil - ParticleComponent.cpp
InversePalindrome.com
*/


#include "ParticleComponent.hpp"
#include "EffectParser.hpp"

#include <Thor/Math/Distributions.hpp>


ParticleComponent::ParticleComponent(ResourceManager& resourceManager, const sf::Vector2f& effectRange, 
	const std::string& particleFile, const std::string& emitterFile) :
	Component("Particle"),
	effectRange(effectRange),
	particleFile(particleFile),
	emitterFile(emitterFile),
	emitter(std::move(Parsers::parseEmitter(emitterFile)))
{
	Parsers::parseParticleSystem(resourceManager, particleFile, particleSystem);
    
	particleSystem.addEmitter(thor::refEmitter(emitter));
}

std::ostream& operator<<(std::ostream& os, const ParticleComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.effectRange.x << ' ' 
		<< component.effectRange.y << ' ' << component.particleFile << ' ' << component.emitterFile;

	return os;
}

void ParticleComponent::update(float deltaTime)
{
	this->particleSystem.update(sf::seconds(deltaTime));
}

sf::FloatRect ParticleComponent::getGlobalBounds() const
{
	return sf::FloatRect(this->getPosition(), { this->effectRange.x, this->effectRange.y });
}

void ParticleComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = this->getTransform();

	target.draw(this->particleSystem, states);
}