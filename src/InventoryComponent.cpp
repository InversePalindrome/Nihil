/*
Copyright (c) 2017 InversePalindrome
Nihil - InventoryComponent.cpp
InversePalindrome.com
*/


#include "InventoryComponent.hpp"

InventoryComponent::InventoryComponent() :
	Component("Inventory")
{
}

std::ostream& operator<<(std::ostream& os, const InventoryComponent& component)
{
	os << component.getEntityID() << ' ' << component.getName();

	return os;
}

std::size_t& InventoryComponent::operator[](Item item)
{
	return (*this->items)[item];
}

const std::size_t& InventoryComponent::operator[](Item item) const
{
	return (*this->items)[item];
}

void InventoryComponent::setItems(Items& items)
{
	this->items = &items;
}

void InventoryComponent::addItem(Item item, std::size_t quantity)
{
	if (items)
	{
		this->items->emplace(item, quantity);
	}
}

void InventoryComponent::removeItem(Item item)
{
	if (items)
	{
		this->items->erase(item);
	}
}

bool InventoryComponent::hasItem(Item item) const
{
	if (items)
	{
		return this->items->count(item);
	}
	else
	{
		return false;
	}
}
