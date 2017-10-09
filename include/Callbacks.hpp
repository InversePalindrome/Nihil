/*
Copyright (c) 2017 InversePalindrome
Nihil - Callbacks.hpp
InversePalindrome.com
*/


#pragma once

#include <vector>
#include <functional>


class Callbacks
{
public:
	void update();

	void addCallback(std::function<void()> callback);
	void clear();;
private:
	std::vector<std::function<void()>> callbacks;
};
