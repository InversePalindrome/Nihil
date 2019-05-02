/*
Copyright (c) 2017 InversePalindrome
Nihil - FrictionUtility.cpp
InversePalindrome.com
*/


#include "FrictionUtility.hpp"


void Utility::setFriction(Entity entity, float friction)
{
    if (entity.has_component<PhysicsComponent>())
    {
        auto& physics = entity.get_component<PhysicsComponent>();

        for (const auto* edge = physics.getBody()->GetContactList(); edge; edge = edge->next)
        {
            edge->contact->SetFriction(friction);
        }
    }
}

void Utility::setFriction(const CollisionData* collisionData, b2Contact* contact, float friction)
{
    if (collisionData->entity.has_component<PhysicsComponent>())
    {
        if (collisionData->entity.get_component<PhysicsComponent>().isMidAir())
        {
            contact->SetFriction(friction);

            if (friction > 0.f)
            {
                for (const auto* edge = collisionData->fixture->GetBody()->GetContactList(); edge; edge = edge->next)
                {
                    edge->contact->ResetFriction();
                }
            }
        }
    }
}