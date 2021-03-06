/*
Copyright (c) 2017 InversePalindrome
Nihil - StateMachine.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"
#include "StateID.hpp"
#include "StateData.hpp"

#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>


class StateMachine
{
    using StatePtr = std::unique_ptr<State>;

public:
    StateMachine(StateData& stateData);

    void handleEvent(const sf::Event& event);
    void update(float deltaTime);
    void draw();

    StatePtr& operator[](std::size_t statePosition);
    std::size_t size() const;

    void changeState(StateID stateID);

    void pushState(StateID stateID);
    void popState();
    void clearStates();

    template<typename T>
    void registerState(StateID stateID);

private:
    StateData& stateData;

    std::vector<std::pair<StateID, StatePtr>> states;
    std::vector<std::function<void()>> stateActions;
    std::unordered_map<StateID, std::function<StatePtr()>> stateFactory;

    StatePtr getState(StateID stateID);

    void processStateActions();
};


template<typename T>
void StateMachine::registerState(StateID stateID)
{
    this->stateFactory[stateID] = [this]()
    {
        return std::make_unique<T>(*this, this->stateData);
    };
}