/*
Copyright (c) 2017 InversePalindrome
Nihil - PickupComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Item.hpp"
#include "ResourceManager.hpp"


class PickupComponent : public Component
{
    friend std::ostream& operator<<(std::ostream& os, const PickupComponent& component);

public:
    PickupComponent(Item item, SoundBuffersID soundID);

    Item getItem() const;
    SoundBuffersID getSoundID() const;

    void setItem(Item item);
    void setSoundID(SoundBuffersID soundID);

private:
    Item item;
    SoundBuffersID soundID;
};

std::ostream& operator<<(std::ostream& os, const PickupComponent& component);
