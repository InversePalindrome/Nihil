/*
Copyright (c) 2017 InversePalindrome
Nihil - ResourceManager.hpp
InversePalindrome.com
*/


#pragma once

#include <Thor/Resources/ResourceHolder.hpp>
#include <Thor/Resources/ResourceLoader.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

 
enum class TexturesID { SplashScreen, StartBackground, Particles };
enum class ImagesID {};
enum class SoundsID {};

struct ResourceManager
{
	thor::ResourceHolder<sf::Texture, TexturesID> textures;
	thor::ResourceHolder<sf::Image, ImagesID> images;
	thor::ResourceHolder<sf::SoundBuffer, SoundsID> sounds;
};