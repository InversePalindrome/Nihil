/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionHandler.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "CollisionData.hpp"

#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>

#include <utility>
#include <optional>


class CollisionHandler : public b2ContactListener
{
	using OrderedCollision = std::pair<std::reference_wrapper<CollisionData>, std::reference_wrapper<CollisionData>>;
	using Collider = std::reference_wrapper<CollisionData>;

public:
	CollisionHandler(Events& events);

private:
    Events& events;

	virtual void BeginContact(b2Contact* contact) override;
	virtual void EndContact(b2Contact* contact) override;

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
	
	std::optional<Collider> getCollider(CollisionData* objectA, CollisionData* objectB, ObjectType type);
	
	std::optional<OrderedCollision> getOrderedCollision(CollisionData* objectA, CollisionData* objectB,
		ObjectType type1, ObjectType type2);
};