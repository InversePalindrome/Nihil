/*
Copyright (c) 2017 InversePalindrome
Nihil - PickupComponent.cpp
InversePalindrome.com
*/


#include "PickupComponent.hpp"


PickupComponent::PickupComponent(Item item, SoundBuffersID soundID) :
    Component("Pickup"),
    item(item),
    soundID(soundID)
{
}

std::ostream& operator<<(std::ostream& os, const PickupComponent& component)
{
    os << component.getEntityID() << ' ' << component.getName() << ' ' << static_cast<std::size_t>(component.item)
        << ' ' << static_cast<std::size_t>(component.soundID);

    return os;
}

Item PickupComponent::getItem() const
{
    return this->item;
}

SoundBuffersID PickupComponent::getSoundID() const
{
    return this->soundID;
}

void PickupComponent::setItem(Item item)
{
    this->item = item;
}

void PickupComponent::setSoundID(SoundBuffersID soundID)
{
    this->soundID = soundID;
}
