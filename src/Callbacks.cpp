/*
Copyright (c) 2017 InversePalindrome
Nihil - Callbacks.cpp
InversePalindrome.com
*/


#include "Callbacks.hpp"


void Callbacks::update()
{
	for (auto& callback : this->callbacks)
	{
		callback();
	}
}

void Callbacks::addCallback(std::function<void()> callback)
{
	this->callbacks.push_back(callback);
}

void Callbacks::clear()
{
	this->callbacks.clear();
}