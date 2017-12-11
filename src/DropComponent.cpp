/*
Copyright (c) 2017 InversePalindrome
Nihil - DropComponent.cpp
InversePalindrome.com
*/


#include "DropComponent.hpp"
#include "FilePaths.hpp"

#include <Thor/Math/Random.hpp>

#include <fstream>


DropComponent::DropComponent(const std::string& fileName) :
	Component("Drop")
{
	std::ifstream inFile(Path::miscellaneous / fileName);
	std::size_t itemID = 0u;
	float odds = 0.f;

	while (inFile >> itemID >> odds)
	{
		drops.push_back({ Item{itemID}, odds });
	}
}

std::ostream& operator<<(std::ostream& os, const DropComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName();

	return os;
}

std::optional<Item> DropComponent::getDrop() const
{
	auto index = thor::random(0u, this->drops.size() - 1);

	auto odds = thor::random(0.f, 1.f);

	if (!this->drops.empty() && this->drops[index].second >= odds)
	{
		return this->drops[index].first;
	}
	else
	{
		return {};
	}
}