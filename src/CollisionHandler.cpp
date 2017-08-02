/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionHandler.cpp
InversePalindrome.com
*/


#include "CollisionHandler.hpp"
#include "CollisionData.hpp"


CollisionHandler::CollisionHandler(Events& events) :
	events(events)
{
}

void CollisionHandler::BeginContact(b2Contact* contact)
{
	auto* objectA = static_cast<CollisionData*>(contact->GetFixtureA()->GetBody()->GetUserData());
	auto* objectB = static_cast<CollisionData*>(contact->GetFixtureB()->GetBody()->GetUserData());
	
	

}

void CollisionHandler::EndContact(b2Contact* contact)
{
}

void CollisionHandler::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}

void CollisionHandler::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) 
{

}