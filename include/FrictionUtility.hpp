/*
Copyright (c) 2017 InversePalindrome
Nihil - FrictionUtility.hpp
InversePalindrome.com
*/


#pragma once


#include "CollisionData.hpp"

#include <Box2D/Dynamics/Contacts/b2Contact.h>


namespace Utility
{
	void setFriction(Entity entity, float friction);
	void setFriction(const CollisionData* collisionData, b2Contact* contact, float friction);
}