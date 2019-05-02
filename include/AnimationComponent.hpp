/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Animation.hpp"
#include "Component.hpp"

#include <Thor/Animations/Animator.hpp>
#include <Thor/Animations/FrameAnimation.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <unordered_map>


class AnimationComponent : public Component
{
    friend std::ostream& operator<<(std::ostream& os, const AnimationComponent& component);

public:
    AnimationComponent(const std::string& animationsFile);

    void setAnimations(const std::string& animationsFile);

    void update(float deltaTime);
    void animate(sf::Sprite& sprite);

    void playAnimation(const Animation& animation, bool loop);
    void stopAnimation();

    bool isPlayingAnimation() const;
    bool hasAnimation(const Animation& animation) const;

private:
    std::string animationsFile;
    Animator animator;
    Animations animations;
};

std::ostream& operator<<(std::ostream& os, const AnimationComponent& component);