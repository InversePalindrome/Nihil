/*
(c) 2017 InversePalindrome
Nihil - Component.hpp
InversePalindrome.com
*/


#pragma once

#include <string>
#include <fstream>


class Component
{
public:
	Component(const std::string& componentID);

    std::string getName() const;
	std::size_t getEntity() const;

	void setEntity(std::size_t entity);

private:
	std::string name;
	std::size_t entity;
};
