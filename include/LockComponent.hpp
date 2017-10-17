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
	LockComponent(std::size_t unlockID, const std::string& newSpriteFile);

	std::size_t getUnlockID() const;
	std::string getNewSpriteFile() const;

	void setUnlockID(std::size_t unlockID);

private:
	std::size_t unlockID;
	std::string newSpriteFile;
};

std::ostream& operator<<(std::ostream& os, const LockComponent& component);