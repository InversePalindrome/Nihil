/*
Copyright (c) 2017 InversePalindrome
Nihil - ControllableComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"


class ControllableComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const ControllableComponent& component);

public:
	ControllableComponent();

	void setMidAirStatus(bool midAirStatus);
	void setConversationStatus(bool conversationStatus);
	void setCurrentConversationID(std::size_t currentConversationID);

	bool isMidAir() const;
	bool canConversate() const;
	std::size_t getCurrentConversationID() const;

private:
	bool midAirStatus;
	bool conversationStatus;
	std::size_t currentConversationID;
};

std::ostream& operator<<(std::ostream& os, const ControllableComponent& component);
