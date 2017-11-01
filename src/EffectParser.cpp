/*
Copyright (c) 2017 InversePalindrome
Nihil - EffectParser.cpp
InversePalindrome.com
*/


#include "EffectParser.hpp"
#include "FilePaths.hpp"
#include "AnimationParser.hpp"

#include <Thor/Math/Distributions.hpp>
#include <Thor/Particles/Affectors.hpp>
#include <Thor/Animations/FadeAnimation.hpp>
#include <Thor/Animations/ColorAnimation.hpp>

#include <fstream>
#include <sstream>


void Parsers::parseParticleSystem(ResourceManager& resourceManager, const std::string& fileName, thor::ParticleSystem& particleSystem)
{
	std::ifstream inFile(Path::particles / fileName);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string category;

		iStream >> category;
		
		if (category == "Texture")
		{
			std::size_t textureID = 0u;

			iStream >> textureID;

			particleSystem.setTexture(resourceManager.getTexture(static_cast<TexturesID>(textureID)));
		}
		else if (category == "SubRect")
		{
			std::size_t left = 0u, top = 0u, width = 0u, height = 0u;

			iStream >> left >> top >> width >> height;

			particleSystem.addTextureRect(sf::IntRect(left, top, width, height));
		}
		else if(category == "ForceAffector")
		{
			float xForce = 0.f, yForce = 0.f;

			iStream >> xForce >> yForce;

			particleSystem.addAffector(thor::ForceAffector({ xForce, yForce }));
		}
		else if (category == "ScaleAffector")
		{
			float xScale = 0.f, yScale = 0.f;

			iStream >> xScale >> yScale;

			particleSystem.addAffector(thor::ScaleAffector({ xScale, yScale }));
		}
		else if (category == "TorqueAffector")
		{
			float angularAcceleration = 0.f;

			iStream >> angularAcceleration;

			particleSystem.addAffector(thor::TorqueAffector(angularAcceleration));
		}
		else if (category == "FadeAffector")
		{
			float startingFade = 0.f, finalFade = 0.f;

			iStream >> startingFade >> finalFade;

			particleSystem.addAffector(thor::AnimationAffector(thor::FadeAnimation(startingFade, finalFade)));
		}
		else if (category == "ColorAffector")
		{
			std::string colorFile;

			iStream >> colorFile;

			particleSystem.addAffector(thor::AnimationAffector(thor::ColorAnimation(Parsers::parseColors(colorFile))));
		}
	}
}

thor::UniversalEmitter Parsers::parseEmitter(const std::string& fileName)
{
	thor::UniversalEmitter emitter;

	std::ifstream inFile(Path::particles / fileName);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string category;

		iStream >> category;

		if (category == "EmissionRate")
		{
			float minParticlesPerSecond = 0.f, maxParticlesPerSecond = 0.f;

			iStream >> minParticlesPerSecond >> maxParticlesPerSecond;

			emitter.setEmissionRate(thor::Distributions::uniform(minParticlesPerSecond, maxParticlesPerSecond)());
		}
		else if (category == "Lifetime")
		{
			float startingTime = 0.f, finalTime = 0.f;

			iStream >> startingTime >> finalTime;

			emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(startingTime), sf::seconds(finalTime)));
		}
		else if (category == "Position")
		{
			float centerX = 0.f, centerY = 0.f, halfWidth = 0.f, halfHeight = 0.f;

			iStream >> centerX >> centerY >> halfWidth >> halfHeight;

			emitter.setParticlePosition(thor::Distributions::rect({ centerX, centerY }, { halfWidth, halfHeight }));
		}
		else if (category == "Velocity")
		{
			float meanXVelocity = 0.f, meanYVelocity = 0.f, deltaXVelocity = 0.f, deltaYVelocity = 0.f;

			iStream >> meanXVelocity >> meanYVelocity >> deltaXVelocity >> deltaYVelocity;

			emitter.setParticleVelocity(thor::Distributions::rect({ meanXVelocity, meanYVelocity }, { deltaXVelocity, deltaYVelocity }));
		}
		else if (category == "Rotation")
		{
			float initialMinRotation = 0.f, initialMaxRotation = 0.f;

			iStream >> initialMinRotation >> initialMaxRotation;

			emitter.setParticleRotation(thor::Distributions::uniform(initialMinRotation, initialMaxRotation));
		}
		else if (category == "RotationSpeed")
		{
			float minRotationSpeed = 0.f, maxRotationSpeed = 0.f;

			iStream >> minRotationSpeed >> maxRotationSpeed;

			emitter.setParticleRotationSpeed(thor::Distributions::uniform(minRotationSpeed, maxRotationSpeed));
		}
		else if (category == "Scale")
		{
			float meanXScale = 0.f, meanYScale = 0.f, deltaXScale = 0.f, deltaYScale = 0.f;

			iStream >> meanXScale >> meanYScale >> deltaXScale >> deltaYScale;

			emitter.setParticleScale(thor::Distributions::rect({ meanXScale, meanYScale }, { deltaXScale, deltaYScale }));
		}
		else if (category == "Color")
		{
			sf::Uint8 R = 0u, G = 0u, B = 0u;

			iStream >> R >> G >> B;
		
			emitter.setParticleColor(sf::Color(R, G, B));
		}
		else if (category == "TextureIndex")
		{
			std::size_t numberOfParticles = 0u;

			iStream >> numberOfParticles;

			emitter.setParticleTextureIndex(thor::Distributions::uniform(0u, numberOfParticles - 1u));
		}
	}

	return emitter;
}