/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionHandler.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"

#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>


class CollisionHandler : public b2ContactListener
{
public:
	CollisionHandler(Events& events);

private:
	Events& events;

	virtual void BeginContact(b2Contact* contact) override;
	virtual void EndContact(b2Contact* contact) override;

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override; 
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};