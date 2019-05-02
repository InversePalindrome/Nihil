/*
Copyright (c) 2017 InversePalindrome
Nihil - SplashState.cpp
InversePalindrome.com
*/


#include "SplashState.hpp"
#include "StateMachine.hpp"


SplashState::SplashState(StateMachine& stateMachine, StateData& stateData) :
    State(stateMachine, stateData),
    splashTime(3u),
    splashScreen(stateData.resourceManager.getTexture(TexturesID::SplashScreen))
{
    splashScreen.setOrigin(splashScreen.getLocalBounds().width / 2.f, splashScreen.getLocalBounds().height / 2.f);
    splashScreen.setPosition(stateData.window.getSize().x / 2.f, stateData.window.getSize().y / 2.f);
}

void SplashState::handleEvent(const sf::Event & event)
{

}

void SplashState::update(float deltaTime)
{
    this->splashTime -= deltaTime;

    if (this->splashTime <= 0.f)
    {
        this->stateMachine.changeState(StateID::Start);
    }
}

void SplashState::draw()
{
    this->stateData.window.draw(splashScreen);
}