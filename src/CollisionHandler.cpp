/*
Copyright (c) 2017 InversePalindrome
Nihil - CollisionHandler.cpp
InversePalindrome.com
*/


#include "CollisionHandler.hpp"
#include "FrictionUtility.hpp"


CollisionHandler::CollisionHandler(Events& events) :
	events(events)
{
}

void CollisionHandler::BeginContact(b2Contact* contact)
{
	auto* objectA = static_cast<CollisionData*>(contact->GetFixtureA()->GetUserData());
	auto* objectB = static_cast<CollisionData*>(contact->GetFixtureB()->GetUserData());

	if (!objectA || !objectB)
	{
		return;
	}

	if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Border))
	{
		const auto& alive = orderedCollision->first;

		this->events.broadcast(DestroyEntity{ alive.entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Checkpoint))
	{
		const auto& checkpoint = orderedCollision->second;

		this->events.broadcast(CrossedCheckpoint{ { checkpoint.properties.at("xPosition").getFloatValue(), checkpoint.properties.at("yPosition").getFloatValue() } });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Enemy))
	{
		const auto& [player, enemy] = *orderedCollision;

		this->events.broadcast(CombatOcurred{ enemy.entity, player.entity });
		this->events.broadcast(ApplyKnockback{ enemy.entity, player.entity });
		this->events.broadcast(ChangeState{ enemy.entity, EntityState::Attacking });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Portal))
	{
		const auto& portal = orderedCollision->second;

		this->events.broadcast(ChangeLevel{ portal.properties.at("Destination").getStringValue(), 
			{ portal.properties.at("xPosition").getFloatValue(), portal.properties.at("yPosition").getFloatValue() } });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Teleporter))
	{
		const auto& [player, teleporter] = *orderedCollision;

		this->events.broadcast(SetPosition{ player.entity,
		{ teleporter.properties.at("xPosition").getFloatValue(), teleporter.properties.at("yPosition").getFloatValue()} });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Pickup))
	{
		const auto& [player, pickup] = *orderedCollision;

		this->events.broadcast(PickedUpItem{ player.entity, pickup.entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Movable, ObjectType::Trampoline))
	{
		const auto& [movable, trampoline] = *orderedCollision;

		this->events.broadcast(ApplyImpulse{ movable.entity, { 0.f, trampoline.properties.at("Impulse").getFloatValue() } });
		this->events.broadcast(PlaySound{ SoundBuffersID::Trampoline, false });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Waypoint))
	{
		const auto& alive = orderedCollision->first;

		this->events.broadcast(CrossedWaypoint{ alive.entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Bullet, ObjectType::Alive))
	{
		const auto& [projectile, alive] = *orderedCollision;
		
		this->events.broadcast(CombatOcurred{ projectile.entity, alive.entity });
		this->events.broadcast(StopMovement{ alive.entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Explosion, ObjectType::Alive))
	{
		const auto& [explosion, alive] = *orderedCollision;
		
		this->events.broadcast(ApplyBlastImpact{ explosion.entity, alive.entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Character))
	{
		const auto& character = orderedCollision->second;

		this->events.broadcast(DisplayConversation{ character.entity, true });
		this->events.broadcast(UpdateConversation{ character.entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Spike))
	{
		const auto& alive = orderedCollision->first;

		this->events.broadcast(DestroyEntity{ alive.entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Head, ObjectType::Liquid))
	{
		const auto& alive = orderedCollision->first;

		this->events.broadcast(AddUnderWaterTimer{ alive.entity });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Block))
	{
		const auto& alive = orderedCollision->first;

		this->events.broadcast(SetMidAirStatus{ alive.entity, false });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Ice))
	{
		const auto& alive = orderedCollision->first;

		this->events.broadcast(SetMidAirStatus{ alive.entity, false });
		this->events.broadcast(SetFriction{ alive.entity, ObjectType::Player, 0.f });

		Utility::setFriction(&orderedCollision->first, contact, 0.f);
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Character))
	{
		const auto& character = orderedCollision->second;

		this->events.broadcast(DisplayConversation{ character.entity, true });
	}

	if (auto collider = this->getCollider(objectA, objectB, ObjectType::Player))
	{
		Utility::setFriction(&collider.value(), contact, 0.f);
	}
	if (auto collider = this->getCollider(objectA, objectB, ObjectType::Bullet))
	{
		this->events.broadcast(DestroyEntity{ collider->entity });
	}
	else if (auto collider = this->getCollider(objectA, objectB, ObjectType::Bomb))
	{
		this->events.broadcast(ActivateBomb{ collider->entity });
	}
}

void CollisionHandler::EndContact(b2Contact* contact)
{
	auto* objectA = static_cast<CollisionData*>(contact->GetFixtureA()->GetUserData());
	auto* objectB = static_cast<CollisionData*>(contact->GetFixtureB()->GetUserData());

	if (!objectA || !objectB)
	{
		return;
	}
	
	if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Player, ObjectType::Character))
	{
		const auto& character = orderedCollision->second;

		this->events.broadcast(DisplayConversation{ character.entity, false });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Block))
	{
		const auto& alive = orderedCollision->first;

		this->events.broadcast(SetMidAirStatus{ alive.entity, true });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Feet, ObjectType::Ice))
	{
		const auto& alive = orderedCollision->first;

		this->events.broadcast(SetMidAirStatus{ alive.entity, true });
		this->events.broadcast(SetFriction{ alive.entity, ObjectType::Player, 0.3f });
	}
	else if (const auto& orderedCollision = this->getOrderedCollision(objectA, objectB, ObjectType::Head, ObjectType::Liquid))
	{
		const auto& alive = orderedCollision->first;

		this->events.broadcast(RemoveUnderWaterTimer{ alive.entity });
		this->events.broadcast(SetUnderWaterStatus{ alive.entity, false });
		this->events.broadcast(SetGravityScale{ alive.entity, 1.f });
		this->events.broadcast(SetLinearDamping{ alive.entity, 0.f });
		this->events.broadcast(PropelFromWater{ alive.entity });
	}
}

void CollisionHandler::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}

void CollisionHandler::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}

std::optional<CollisionData> CollisionHandler::getCollider(CollisionData* objectA, CollisionData* objectB, ObjectType type)
{
	if (objectA->objectType & type)
	{
		return *objectA;
	}
	else if (objectB->objectType & type)
	{
		return *objectB;
	}
	else
	{
		return {};
	}
}

std::optional<CollisionHandler::OrderedCollision> CollisionHandler::getOrderedCollision(CollisionData* objectA,
	CollisionData* objectB, ObjectType type1, ObjectType type2)
{
	if (objectA->objectType & type1 && objectB->objectType & type2)
	{
		return OrderedCollision(*objectA, *objectB);
	}
	else if (objectA->objectType & type2 && objectB->objectType & type1)
	{
		return OrderedCollision(*objectB, *objectA);
	}
	else
	{
		return {};
	}
}