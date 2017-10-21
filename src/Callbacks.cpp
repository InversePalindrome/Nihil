/*
Copyright (c) 2017 InversePalindrome
Nihil - Callbacks.cpp
InversePalindrome.com
*/


#include "Callbacks.hpp"


void Callbacks::update()
{
	for (auto& callback : this->callbacks)
	{
		callback();
	}

	for (auto callbackTimer = std::begin(this->callbackTimers); callbackTimer != std::end(this->callbackTimers); )
	{
		callbackTimer->update();

		if (callbackTimer->isExpired())
		{
			callbackTimer = this->callbackTimers.erase(callbackTimer);
		}
		else
		{
			++callbackTimer;
		}
	}
}

void Callbacks::addCallback(std::function<void()> callback)
{
	this->callbacks.push_back(callback);
}

void Callbacks::addCallbackTimer(std::function<void()> callback, float callbackTime)
{
	this->callbackTimers.push_back(thor::CallbackTimer());
	this->callbackTimers.back().connect0(callback);
	this->callbackTimers.back().restart(sf::seconds(callbackTime));
}

void Callbacks::clearCallbacks()
{
	this->callbacks.clear();
}

void Callbacks::clearCallbackTimers()
{
	this->callbackTimers.clear();
}

void Callbacks::disconnectCallbackTimers()
{
	for (auto& callback : this->callbackTimers)
	{
		callback.clearConnections();
	}
}