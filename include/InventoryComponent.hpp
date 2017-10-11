/*
Copyright (c) 2017 InversePalindrome
Nihil - InventoryComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Item.hpp"


class InventoryComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const InventoryComponent& component);

public:
	InventoryComponent();

	std::size_t& operator[](Item item);
	const std::size_t& operator[](Item item) const;

	void setItems(Items& items);

	void addItem(Item item, std::size_t quantity);
	void removeItem(Item item);

	bool hasItem(Item item) const;

private:
	Items* items;
};

std::ostream& operator<<(std::ostream& os, const InventoryComponent& component);