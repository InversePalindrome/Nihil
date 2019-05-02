/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimatorSystem.hpp
InversePalindrome.com
*/


#pragma once

#include "System.hpp"
#include "Animation.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


class AnimatorSystem : public System
{
public:
    AnimatorSystem(Entities& entities, Events& events);

    virtual void update(float deltaTime) override;

    void animate(sf::RenderTarget& target);

private:
    void playAnimation(Entity entity, const Animation& animation, bool loop);
    void stopAnimation(Entity entity);

    void changeAnimationState(Entity entity, EntityState state);
    void changeAnimationDirection(Entity entity, Direction direction);

    void playStartingAnimation(Entity entity, AnimationComponent& animation);
};