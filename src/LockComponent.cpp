/*
Copyright (c) 2017 InversePalindrome
Nihil - LockComponent.cpp
InversePalindrome.com
*/


#include "LockComponent.hpp"


LockComponent::LockComponent(std::size_t unlockID, const std::string& newSpriteFile) :
	Component("Lock"),
	unlockID(unlockID),
	newSpriteFile(newSpriteFile)
{
}

std::ostream& operator<<(std::ostream& os, const LockComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.unlockID << ' ' << component.newSpriteFile;

	return os;
}

std::size_t LockComponent::getUnlockID() const
{
	return this->unlockID;
}

std::string LockComponent::getNewSpriteFile() const
{
	return this->newSpriteFile;
}

void LockComponent::setUnlockID(std::size_t unlockID)
{
	this->unlockID = unlockID;
}