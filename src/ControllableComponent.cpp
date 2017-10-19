/*
Copyright (c) 2017 InversePalindrome
Nihil - ControllableComponent.cpp
InversePalindrome.com
*/


#include "ControllableComponent.hpp"


ControllableComponent::ControllableComponent() :
	Component("Controllable"),
	midAirStatus(false),
	conversationStatus(false),
	currentConversationID(0u)
{
}

std::ostream& operator<<(std::ostream& os, const ControllableComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName();

	return os;
}

void ControllableComponent::setMidAirStatus(bool midAirStatus)
{
	this->midAirStatus = midAirStatus;
}

void ControllableComponent::setConversationStatus(bool conversationStatus)
{
	this->conversationStatus = conversationStatus;
}

void ControllableComponent::setCurrentConversationID(std::size_t currentConversationID)
{
	this->currentConversationID = currentConversationID;
}

bool ControllableComponent::isMidAir() const
{
	return this->midAirStatus;
}

bool ControllableComponent::canConversate() const
{
	return this->conversationStatus;
}

std::size_t ControllableComponent::getCurrentConversationID() const
{
	return this->currentConversationID;
}