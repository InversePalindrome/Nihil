/*
Copyright (c) 2017 InversePalindrome
Nihil - InventoryComponent.cpp
InversePalindrome.com
*/


#include "InventoryComponent.hpp"

#include <fstream>
#include <sstream>


InventoryComponent::InventoryComponent(const std::string& pathFile) :
	Component("Inventory"),
	pathFile(pathFile)
{
	std::ifstream inFile(pathFile);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::size_t itemName = 0u, quantity = 0u;

		iStream >> itemName >> quantity;

		this->inventory.emplace(static_cast<Item>(itemName), quantity);
	}
}

std::ostream& operator<<(std::ostream& os, const InventoryComponent& component)
{
	std::ofstream outFile(component.pathFile);

	for (const auto& item : component.inventory)
	{
		outFile << static_cast<std::size_t>(item.first) << ' ' << item.second << '\n';
	}

	os << component.getEntityID() << ' ' << component.getName() << ' ' << component.pathFile;

	return os;
}

std::size_t& InventoryComponent::operator[](Item item)
{
	return this->inventory[item];
}

void InventoryComponent::addItem(Item item, std::size_t quantity)
{
	this->inventory.emplace(item, quantity);
}

void InventoryComponent::removeItem(Item item)
{
	this->inventory.erase(item);
}

bool InventoryComponent::hasItem(Item item) const
{
	return this->inventory.count(item);
}
