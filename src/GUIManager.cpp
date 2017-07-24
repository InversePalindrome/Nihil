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
	this->hud.Add(widget);
}

void GUIManager::removeWidget(widgetPtr widget)
{
	this->hud.Remove(widget);
}

void GUIManager::removeAllWidgets()
{
	this->hud.RemoveAll();
}