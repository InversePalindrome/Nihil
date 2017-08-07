/*
Copyright (c) 2017 InversePalindrome
Nihil - StateMachine.cpp
InversePalindrome.com
*/


#include "StateMachine.hpp"


StateMachine::StateMachine(StateData& stateData) :
	stateData(stateData),
	states(),
	stateActions(),
	stateFactory()
{
}

void StateMachine::handleEvent(const sf::Event& event)
{
	if (!this->states.empty())
	{
		this->states.back()->handleEvent(event);
	}

	this->processStateActions();
}

void StateMachine::update(float deltaTime)
{
	if (!this->states.empty())
	{
		this->states.back()->update(deltaTime);
	}

	this->processStateActions();
}

void StateMachine::draw()
{
	if (!this->states.empty())
	{
		if (this->states.back()->isTransparent() && this->states.size() > 1)
		{
			auto itr = std::end(this->states);

			for (; itr != std::begin(this->states); --itr)
			{
				if (itr != std::end(this->states) && !(*itr)->isTransparent())
				{
					break;
				}
			}

			for (; itr != std::end(this->states); ++itr)
			{
				(*itr)->draw();
			}
		}
		else
		{
			this->states.back()->draw();
		}
	}
}

void StateMachine::changeState(StateID stateID)
{
	this->stateData.soundManager.stopAllMusic();

	this->popState();
	this->pushState(stateID);
}

void StateMachine::pushState(StateID stateID)
{
	this->stateData.guiManager.hideAllWidgets();

	this->stateActions.push_back([this, stateID] { this->states.push_back(this->getState(stateID)); });
}

void StateMachine::popState()
{
	this->stateData.guiManager.hideAllWidgets();

	this->stateActions.push_back([this] { this->states.pop_back(); });
}

void StateMachine::clearStates()
{
	this->stateData.guiManager.hideAllWidgets();
	this->stateData.soundManager.stopAllMusic();

	this->stateActions.push_back([this] { this->states.clear(); });
}

StateMachine::StatePtr StateMachine::getState(StateID stateID)
{
	return this->stateFactory.find(stateID)->second();
}

void StateMachine::processStateActions()
{
	for (const auto& action : this->stateActions)
	{
		action();
	}

	this->stateActions.clear();
}