/*
Copyright (c) 2017 InversePalindrome
Nihil - GUIManager.hpp
InversePalindrome.com
*/


#pragma once

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Desktop.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <memory>


class GUIManager
{
	using widgetPtr = std::shared_ptr<sfg::Widget>;

public:
	GUIManager(sf::RenderWindow& window);

	void update(float deltaTime);
	void handleEvent(const sf::Event& event);
	void display();

	void addWidget(widgetPtr widget);

	void hideAllWidgets();

	template<typename T>
	void setProperty(const std::string& selector, const std::string& property, const T& value);

private:
	std::vector<widgetPtr> widgets;
	sfg::SFGUI gui;
	sfg::Desktop hud;

	sf::RenderWindow& window;
};


template<typename T>
void GUIManager::setProperty(const std::string& selector, const std::string& property, const T& value)
{
	this->hud.SetProperty(selector, property, value);
}