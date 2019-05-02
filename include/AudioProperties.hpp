/*
Copyright (c) 2017 InversePalindrome
Nihil - AudioProperties.hpp
InversePalindrome.com
*/


#pragma once

#include <string>


struct AudioProperties
{
    AudioProperties(const std::string& fileName);
    AudioProperties(float volume, float pitch, float attenuation, float minDistance);

    void saveData(const std::string& fileName) const;

    float volume;
    float pitch;
    float attenuation;
    float minDistance;
};