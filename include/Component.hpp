/*
(c) 2017 InversePalindrome
Nihil - Component.hpp
InversePalindrome.com
*/


#pragma once

#include <string>
#include <fstream>
#include <cstddef>


class Component
{
public:
	Component(const std::string& componentID);

    std::string getName() const;
	std::int32_t getEntityID() const;

	void setEntityID(std::int32_t entity);

private:
	std::string name;
	std::int32_t entity;
};
