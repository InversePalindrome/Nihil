/*
Copyright (c) 2017 InversePalindrome
Nihil - AttackComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <cstdint>


class MeleeAttackComponent : public Component
{
    friend std::ostream& operator<<(std::ostream& os, const MeleeAttackComponent& component);

public:
    MeleeAttackComponent(std::int32_t damagePoints);
    MeleeAttackComponent(std::int32_t damagePoints, float knockback);

    std::int32_t getDamagePoints() const;
    float getKnockback() const;

    void setDamagePoints(std::int32_t damagePoints);
    void setKnockback(float knockback);

private:
    std::int32_t damagePoints;
    float knockback;
};

std::ostream& operator<<(std::ostream& os, const MeleeAttackComponent& component);