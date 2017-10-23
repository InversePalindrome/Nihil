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
		this->states.back().second->handleEvent(event);
	}

	this->processStateActions();
}

void StateMachine::update(float deltaTime)
{
	if (!this->states.empty())
	{
		if (this->states.back().second->isVisible() && this->states.size() > 1)
		{
			auto itr = std::end(this->states);

			for (; itr != std::begin(this->states); --itr)
			{
				if (itr != std::end(this->states) && !(*itr).second->isVisible())
				{
					break;
				}
			}

			for (; itr != std::end(this->states); ++itr)
			{
				(*itr).second->update(deltaTime);
			}
		}
		else
		{
			this->states.back().second->update(deltaTime);
		}
	}

	this->processStateActions();
}

void StateMachine::draw()
{
	if (!this->states.empty())
	{
		if (this->states.back().second->isTransparent() && this->states.size() > 1)
		{
			auto itr = std::end(this->states);

			for (; itr != std::begin(this->states); --itr)
			{
				if (itr != std::end(this->states) && !(*itr).second->isTransparent())
				{
					break;
				}
			}

			for (; itr != std::end(this->states); ++itr)
			{
				(*itr).second->draw();
			}
		}
		else
		{
			this->states.back().second->draw();
		}
	}
}

StateMachine::StatePtr& StateMachine::operator[](std::size_t statePosition)
{
	return this->states[statePosition].second;
}

std::size_t StateMachine::size() const
{
	return this->states.size();
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

	this->stateActions.push_back([this, stateID] { this->states.push_back({ stateID, this->getState(stateID) }); });
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