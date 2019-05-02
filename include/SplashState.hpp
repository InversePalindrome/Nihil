/*
Copyright (c) 2017 InversePalindrome
Nihil - SplashState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <SFGUI/Label.hpp>

#include <SFML/Graphics/Sprite.hpp>


class SplashState : public State
{
public:
    SplashState(StateMachine& stateMachine, StateData& stateData);

    virtual void handleEvent(const sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void draw() override;

private:
    float splashTime;

    sf::Sprite splashScreen;
};