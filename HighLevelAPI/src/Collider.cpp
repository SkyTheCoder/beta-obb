//------------------------------------------------------------------------------
//
// File Name:	Collider.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Collider.h"

// Systems
#include "GameObject.h"

// Components
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// MapCollision constructor.
// Params:
//   bottom = Whether the bottom is colliding or not.
//   top = Whether the top is colliding or not.
//   left = Whether the left is colliding or not.
//   right = Whether the right is colliding or not.
MapCollision::MapCollision(bool bottom, bool top, bool left, bool right) : bottom(bottom), top(top), left(left), right(right)
{
}

// Collider class - Detects collisions between objects

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new collider component.
// Params:
//   owner = Reference to the object that owns this component.
Collider::Collider(ColliderType type) : Component("Collider"), transform(nullptr), physics(nullptr),
	type(type), handler(nullptr)
{
}

// Set component pointers.
void Collider::Initialize()
{
	// Store the required components for ease of access.
	transform = GetOwner()->GetComponent<Transform>();
	physics = GetOwner()->GetComponent<Physics>();
}

// Check if two objects are colliding and send collision events.
// Params:
//	 other = Reference to the second collider component.
void Collider::CheckCollision(const Collider& other)
{
	// Check if the two colliders are colliding.
	if (IsCollidingWith(other))
	{
		// Run collision event handlers if they exist.
		if (handler != nullptr)
			handler(*GetOwner(), *other.GetOwner());
		if (other.handler != nullptr)
			other.handler(*other.GetOwner(), *GetOwner());
	}
}

// Get the type of this component.
ColliderType Collider::GetType() const
{
	return type;
}

// Sets the collision handler function for the collider.
// Params:
//   handler = A pointer to the collision handler function.
void Collider::SetCollisionHandler(CollisionEventHandler handler_)
{
	handler = handler_;
}

// Sets the map collision handler function for the collider.
// Params:
//   handler = A pointer to the collision handler function.
void Collider::SetMapCollisionHandler(MapCollisionEventHandler mapHandler_)
{
	mapHandler = mapHandler_;
}

// Get the map collision handler function pointer.
MapCollisionEventHandler Collider::GetMapCollisionHandler() const
{
	return mapHandler;
}

//------------------------------------------------------------------------------
