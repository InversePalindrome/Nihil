/*
Copyright (c) 2017 InversePalindrome
Nihil - TimerComponent.cpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <Thor/Time/Timer.hpp>

#include <unordered_map>


class TimerComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const TimerComponent& component);

public:
	TimerComponent(const std::string& pathFile);

	void restartTimer(const std::string& timer);
	void startTimer(const std::string& timer);
	void stopTimer(const std::string& timer);

	bool hasTimerExpired(const std::string& timer) const;

private:
	std::string pathFile;
	std::unordered_map<std::string, std::pair<thor::Timer, float>> timers;
};

std::ostream& operator<<(std::ostream& os, const TimerComponent& component);