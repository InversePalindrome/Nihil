/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatedComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <Thor/Time/Timer.hpp>

#include <vector>
#include <string>


class AutomatedComponent : public Component
{
	using Task = std::pair<std::string, float>;

public:
	AutomatedComponent(const std::string& filePath);

	virtual std::ostream& operator<<(std::ostream& os) override;

	Task getCurrentTask() const;

	void pushNextTask();

	bool hasTasks() const;
	bool hasCurrentTaskExpired() const;

private:
	std::string filePath;
	std::vector<Task> tasks;
	thor::Timer timer;
	std::size_t currentTask;
};