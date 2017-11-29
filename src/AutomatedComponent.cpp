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
	activeStatus(true),
	isPlayingTask(true),
	elapsedTime(0.f)
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
		elapsedTime = tasks.front().second;
	}
}

std::ostream& operator<<(std::ostream& os, const AutomatedComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.fileName;

	return os;
}

void AutomatedComponent::update(float deltaTime)
{
	if (this->isPlayingTask)
	{
		this->elapsedTime -= deltaTime;
	}
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

		this->elapsedTime = this->tasks.front().second;
	}
}

void AutomatedComponent::playCurrentTask()
{
	this->isPlayingTask = true;
}

void AutomatedComponent::stopCurrentTask()
{
	this->isPlayingTask = false;
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
	return this->elapsedTime <= 0.f || !this->isPlayingTask;
}

bool AutomatedComponent::isActive() const
{
	return this->activeStatus;
}