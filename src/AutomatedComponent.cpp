/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatedComponent.cpp
InversePalindrome.com
*/


#include "AutomatedComponent.hpp"
#include "FilePaths.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>


AutomatedComponent::AutomatedComponent(const std::string& fileName) :
	Component("Automated"),
	fileName(fileName),
	currentTask(0u),
	activeStatus(true)
{
	std::ifstream inFile(Path::miscellaneous / fileName);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string function;
		float time = 0.f;

		iStream >> function >> time;

		tasks.push_back({ function, time });
	}

	if (!tasks.empty())
	{
		timer.restart(sf::seconds(tasks.front().second));
	}
}

std::ostream& operator<<(std::ostream& os, const AutomatedComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.fileName;

	return os;
}

AutomatedComponent::Task AutomatedComponent::getCurrentTask() const
{
	return this->tasks.front();
}

void AutomatedComponent::pushNextTask()
{
	if (this->tasks.size() > 1u)
	{
		std::rotate(std::rbegin(this->tasks), std::rbegin(this->tasks) + 1u, std::rend(this->tasks));

		this->timer.restart(sf::seconds(this->tasks.front().second));
	}
}

void AutomatedComponent::playCurrentTask()
{
	this->timer.start();
}

void AutomatedComponent::stopCurrentTask()
{
	this->timer.stop();
}

void AutomatedComponent::setActiveStatus(bool activeStatus)
{
	this->activeStatus = activeStatus;
}

bool AutomatedComponent::hasTasks() const
{
	return !this->tasks.empty();
}

bool AutomatedComponent::hasCurrentTaskExpired() const
{
	return this->timer.isExpired();
}

bool AutomatedComponent::isActive() const
{
	return this->activeStatus;
}