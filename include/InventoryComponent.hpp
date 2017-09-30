/*
Copyright (c) 2017 InversePalindrome
Nihil - InventoryComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Item.hpp"

#include <unordered_map>


class InventoryComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const InventoryComponent& component);

public:
	InventoryComponent(const std::string& pathFile);

	std::size_t& operator[](Item item);

	void addItem(Item item, std::size_t quantity);
	void removeItem(Item item);

	bool hasItem(Item item) const;

private:
	std::string pathFile;
	std::unordered_map<Item, std::size_t> inventory;
};

std::ostream& operator<<(std::ostream& os, const InventoryComponent& component);