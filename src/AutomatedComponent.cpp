/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatedComponent.cpp
InversePalindrome.com
*/


#include "AutomatedComponent.hpp"
#include "FilePaths.hpp"
#include "UnitConverter.hpp"

#include <fstream>
#include <algorithm>


AutomatedComponent::AutomatedComponent() :
	Component("Automated"),
	taskIndex(0u)
{
}

std::ostream& operator<<(std::ostream& os, const AutomatedComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName();

	return os;
}

void AutomatedComponent::loadTasks(const std::string& fileName)
{
	std::ifstream inFile(Path::miscellaneous / fileName);

	std::size_t direction = 0u;
	float xDestination = 0.f, yDestination = 0.f;

	while (inFile >> direction >> xDestination >> yDestination)
	{
		this->tasks.push_back({ Direction{direction}, { UnitConverter::pixelsToMeters(xDestination), UnitConverter::pixelsToMeters(-yDestination) } });
	}
}

void AutomatedComponent::pushNextTask()
{
	if (++this->taskIndex == this->tasks.size())
	{
		this->taskIndex = 0u;
	}
}

AutomatedComponent::Task AutomatedComponent::getCurrentTask() const
{
	return this->tasks[this->taskIndex];
}

bool AutomatedComponent::hasTasks() const
{
	return !this->tasks.empty();
}