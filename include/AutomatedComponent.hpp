/*
Copyright (c) 2017 InversePalindrome
Nihil - AutomatedComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <vector>
#include <string>


class AutomatedComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const AutomatedComponent& component);
	using Task = std::pair<std::string, float>;

public:
	AutomatedComponent(const std::string& fileName);

	void update(float deltaTime);

	Task getCurrentTask() const;

	void pushNextTask();

	void playCurrentTask();
	void stopCurrentTask();

	void setActiveStatus(bool activeStatus);

	bool hasTasks() const;
	bool hasCurrentTaskExpired() const;
	bool isActive() const;

private:
	std::string fileName;
	std::vector<Task> tasks;
	bool isPlayingTask;
	float elapsedTime;
	std::size_t currentTask;

	bool activeStatus;
};

std::ostream& operator<<(std::ostream& os, const AutomatedComponent& component);