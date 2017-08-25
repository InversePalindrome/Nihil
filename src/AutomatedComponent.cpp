/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatedComponent.cpp
InversePalindrome.com
*/


#include "AutomatedComponent.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>


AutomatedComponent::AutomatedComponent(const std::string& pathFile) :
	currentTask(0u)
{
	std::ifstream inFile(pathFile);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string function;
		float time = 0.f;

		iStream >> function >> time;

		tasks.push_back(std::make_pair(function, time));
	}
}

AutomatedComponent::Task AutomatedComponent::getCurrentTask() const
{
	return this->tasks.at(0u);
}

void AutomatedComponent::pushNextTask()
{
	if (this->tasks.size() > 1u)
	{
		std::rotate(std::rbegin(this->tasks), std::rbegin(this->tasks) + 1u, std::rend(this->tasks));

		this->timer.restart(sf::seconds(this->tasks[0u].second));
	}
}

bool AutomatedComponent::hasTasks() const
{
	return !this->tasks.empty();
}

bool AutomatedComponent::hasCurrentTaskExpired() const
{
	return this->timer.isExpired();
}