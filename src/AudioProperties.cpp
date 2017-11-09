/*
Copyright (c) 2017 InversePalindrome
Nihil - AudioProperties.cpp
InversePalindrome.com
*/


#include "AudioProperties.hpp"
#include "FilePaths.hpp"

#include <fstream>


AudioProperties::AudioProperties(const std::string& fileName)
{
	std::ifstream inFile(Path::miscellaneous / fileName);

	inFile >> volume >> pitch >> attenuation >> minDistance;
}

AudioProperties::AudioProperties(float volume, float pitch, float attenuation, float minDistance) :
	volume(volume),
	pitch(pitch),
	attenuation(attenuation),
	minDistance(minDistance)
{
}

void AudioProperties::saveData(const std::string& fileName) const
{
	std::ofstream outFile(Path::miscellaneous / fileName);

	outFile << this->volume << ' ' << this->pitch << ' ' << this->attenuation << ' ' << this->minDistance;
}