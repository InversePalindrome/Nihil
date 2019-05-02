/*
Copyright (c) 2017 InversePalindrome
Nihil - BulletComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "Projectile.hpp"


class BulletComponent : public Component, public Projectile
{
    friend std::ostream& operator<<(std::ostream& os, const BulletComponent& component);

public:
    BulletComponent(std::int32_t damagePoints, SoundBuffersID soundID, float force);

    float getForce() const;

    void setForce(float force);

private:
    float force;
};

std::ostream& operator<<(std::ostream& os, const BulletComponent& component);