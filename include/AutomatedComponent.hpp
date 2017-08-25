/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatedComponent.hpp
InversePalindrome.com
*/


#pragma once

#include <Thor/Time/Timer.hpp>

#include <vector>
#include <string>


class AutomatedComponent
{
	using Task = std::pair<std::string, float>;

public:
	AutomatedComponent(const std::string& filePath);

	Task getCurrentTask() const;

	void pushNextTask();

	bool hasTasks() const;
	bool hasCurrentTaskExpired() const;

private:
	std::vector<Task> tasks;
	thor::Timer timer;
	std::size_t currentTask;
};