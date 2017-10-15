/*
Copyright (c) 2017 InversePalindrome
Nihil - LockComponent.cpp
InversePalindrome.com
*/


#include "LockComponent.hpp"


LockComponent::LockComponent(bool lockState, std::size_t unlockID) :
	Component("Lock"),
	lockState(lockState),
	unlockID(unlockID)
{
}

std::ostream& operator<<(std::ostream& os, const LockComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.lockState << ' ' << component.unlockID;

	return os;
}

bool LockComponent::getLockState() const
{
	return this->lockState;
}

std::size_t LockComponent::getUnlockID() const
{
	return this->unlockID;
}

void LockComponent::setLockState(bool lockState)
{
	this->lockState = lockState;
}

void LockComponent::setUnlockID(std::size_t unlockID)
{
	this->unlockID = unlockID;
}