//------------------------------------------------------------------------------
//
// File Name:	Collider.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
class GameObject;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum ColliderType
{
	ColliderTypePoint,
	ColliderTypeCircle,
	ColliderTypeRectangle,
	ColliderTypeTilemap,
	ColliderTypeLines,
} ColliderType;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Booleans determining collision state for each side of the object.
struct MapCollision
{
	MapCollision(bool bottom, bool top, bool left, bool right);
	bool bottom;
	bool top;
	bool left;
	bool right;
};

// Function pointer for collision event handlers.
// Params:
//   callingObject = The object whose handler is being called.
//   otherObject = The other object that the object collided with.
typedef void(*CollisionEventHandler)(GameObject& callingObject, GameObject& otherObject);

// Function pointer for tilemap collision event handlers.
// Params:
//   object = The object that collided with the map.
//   sides = Collision state for each side of the object.
typedef void(*MapCollisionEventHandler)(GameObject& object,
	const MapCollision& sides);

// Collider class - Detects collisions between objects
class Collider : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new collider component.
	// Params:
	//   type = The type of collider (see the ColliderType enum).
	Collider(ColliderType type);

	// Set component pointers.
	void Initialize() override;

	// Draw collision shape
	virtual void Draw() = 0;

	// Check if two objects are colliding and send collision events.
	// Params:
	//	 other = Reference to the second collider component.
	void CheckCollision(const Collider& other);

	// Perform intersection test between two arbitrary colliders.
	// Params:
	//	 other = Reference to the second collider component.
	virtual bool IsCollidingWith(const Collider& other) const = 0;

	// Get the type of this component.
	ColliderType GetType() const;

	// Sets the collision handler function for the collider.
	// Params:
	//   handler = A pointer to the collision handler function.
	void SetCollisionHandler(CollisionEventHandler handler);

	// Sets the map collision handler function for the collider.
	// Params:
	//   handler = A pointer to the collision handler function.
	void SetMapCollisionHandler(MapCollisionEventHandler mapHandler);
	
	// Get the map collision handler function pointer.
	MapCollisionEventHandler GetMapCollisionHandler() const;

	// Component pointers
	Transform* transform;
	Physics* physics;

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The type of collider used by this component.
	ColliderType type;

	// Function pointer for collision handler
	CollisionEventHandler handler;
	
	// Function pointer for tilemap collision handling
	MapCollisionEventHandler mapHandler;
};

//------------------------------------------------------------------------------
