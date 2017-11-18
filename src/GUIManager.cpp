/*
Copyright (c) 2017 InversePalindrome
Nihil - GUIManager.cpp
InversePalindrome.com
*/


#include "GUIManager.hpp"


GUIManager::GUIManager(sf::RenderWindow& window) :
	window(window)
{
}

void GUIManager::update(float deltaTime)
{
	this->hud.Update(deltaTime);
}

void GUIManager::handleEvent(const sf::Event& event)
{
	this->hud.HandleEvent(event);
}

void GUIManager::display()
{
	this->gui.Display(this->window);
}

void GUIManager::addWidget(widgetPtr widget)
{
	this->widgets.push_back(std::move(widget));
	this->hud.Add(this->widgets.back());
}

void GUIManager::removeAllWidgets()
{
	this->hud.RemoveAll();
}

void GUIManager::hideAllWidgets()
{
	for (auto& widget : this->widgets)
	{
		widget->Show(false);
	}
}