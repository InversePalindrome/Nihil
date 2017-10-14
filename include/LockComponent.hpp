/*
Copyright (c) 2017 InversePalindrome
Nihil - LockComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"


class LockComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const LockComponent& component);

public:
	LockComponent(bool lockState, std::size_t unlockID);

	bool getLockState() const;
	std::size_t getUnlockID() const;

	void setLockState(bool lockState);
	void setUnlockID(std::size_t unlockID);

private:
	bool lockState;
	std::size_t unlockID;
};

std::ostream& operator<<(std::ostream& os, const LockComponent& component);