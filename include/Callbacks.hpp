/*
Copyright (c) 2017 InversePalindrome
Nihil - Callbacks.hpp
InversePalindrome.com
*/


#pragma once

#include <vector>
#include <list>
#include <functional>

#include <Thor/Time/CallbackTimer.hpp>


class Callbacks
{
public:
	void update();

	void addCallback(std::function<void()> callback);
	void addCallbackTimer(std::function<void()> callback, float callbackTime);

	void clearCallbacks();
	void clearCallbackTimers();

	void disconnectCallbackTimers();

private:
	std::vector<std::function<void()>> callbacks;
	std::list<thor::CallbackTimer> callbackTimers;
};
