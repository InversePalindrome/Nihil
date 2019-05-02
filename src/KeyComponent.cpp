/*
Copyright (c) 2017 InversePalindrome
Nihil - KeyComponent.cpp
InversePalindrome.com
*/


#include "KeyComponent.hpp"


KeyComponent::KeyComponent(std::size_t keyID) :
    Component("Key"),
    keyID(keyID)
{
}

std::ostream& operator<<(std::ostream& os, const KeyComponent& component)
{
    os << component.getEntityID() << ' ' << component.getName() << ' ' << component.keyID;

    return os;
}

std::size_t KeyComponent::getKeyID() const
{
    return this->keyID;
}

void KeyComponent::setKeyID(std::size_t keyID)
{
    this->keyID = keyID;
}