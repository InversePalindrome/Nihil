/*
Copyright (c) 2017 InversePalindrome
Nihil - TimerComponent.cpp
InversePalindrome.com
*/


#include "TimerComponent.hpp"

#include <fstream>
#include <sstream>


TimerComponent::TimerComponent() :
	Component("TimerA")
{
}

TimerComponent::TimerComponent(const std::string& pathFile) :
	Component("TimerB"),
	pathFile(pathFile)
{
	std::ifstream inFile(pathFile);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string timerName;
		float durationTime;

		iStream >> timerName >> durationTime;

		timers.emplace(timerName, std::make_pair(thor::Timer(), durationTime));
	}
}

std::ostream& operator<<(std::ostream& os, const TimerComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.pathFile;

	return os;
}

void TimerComponent::addTimer(const std::string& timer, float time)
{
	this->timers.emplace(timer, std::make_pair(thor::Timer(), time));
}

void TimerComponent::removeTimer(const std::string& timer)
{
	this->timers.erase(timer);
}

void TimerComponent::restartTimer(const std::string& timer)
{
	this->timers[timer].first.restart(sf::seconds(this->timers[timer].second));
}

void TimerComponent::startTimer(const std::string& timer)
{
	this->timers[timer].first.start();
}

void TimerComponent::stopTimer(const std::string& timer)
{
	this->timers[timer].first.stop();
}

bool TimerComponent::hasTimer(const std::string& timer) const
{
	return this->timers.count(timer);
}

bool TimerComponent::hasTimerExpired(const std::string& timer) const 
{
	return this->timers.at(timer).first.isExpired();
}