/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationParser.cpp
InversePalindrome.com
*/


#include "AnimationParser.hpp"
#include "FilePaths.hpp"

#include <fstream>
#include <sstream>


Animations Parsers::parseAnimations(const std::string& fileName, Animator& animator)
{
    Animations animations;

    std::ifstream inFile(Path::animations / fileName);
    std::string line;

    std::size_t stateID = 0u, direction = 0u;

    while (std::getline(inFile, line))
    {
        std::istringstream iStream(line);

        std::string category;

        iStream >> category;

        if (category == "Animation")
        {
            float duration = 0.f;

            iStream >> stateID >> direction >> duration;

            animations.emplace(std::make_pair(EntityState{ stateID }, Direction{ direction }), std::make_pair(thor::FrameAnimation(), duration));
        }
        else if (category == "Frame")
        {
            float frameTime = 0.f;
            int left = 0, top = 0, width = 0, length = 0;

            iStream >> frameTime >> left >> top >> width >> length;

            animations.at({ EntityState{ stateID }, Direction{ direction } }).first.addFrame(frameTime, { left, top, width, length });
        }
    }

    for (const auto& [animationID, animationData] : animations)
    {
        animator.addAnimation(animationID, animationData.first, sf::seconds(animationData.second));
    }

    return animations;
}