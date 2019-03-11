//------------------------------------------------------------------------------
//
// File Name:	ColliderTilemap.cpp
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

#include "ColliderTilemap.h"

// Systems
#include "Tilemap.h"
#include <Shapes2D.h>
#include "GameObject.h"
#include <Interpolation.h>

// Components
#include "ColliderRectangle.h"
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for Tilemap collider.
ColliderTilemap::ColliderTilemap() : Collider(ColliderType::ColliderTypeTilemap), map(nullptr)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderTilemap::Clone() const
{
	return new ColliderTilemap(*this);
}

// Debug drawing for colliders.
void ColliderTilemap::Draw()
{
}

// Check for collision between a tilemap and another arbitrary collider.
// Params:
//	 other = Reference to the second collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderTilemap::IsCollidingWith(const Collider& other) const
{
	// Only collision with rectangle colliders is handled.
	if (other.GetType() != ColliderType::ColliderTypeRectangle)
		return false;

	// The physics of the other object.
	Physics* otherPhysics = static_cast<Physics*>(other.GetOwner()->GetComponent<Physics>());

	bool shouldResolve = otherPhysics != nullptr;

	// The transform of the other object.
	Transform* otherTransform = static_cast<Transform*>(other.GetOwner()->GetComponent<Transform>());

	// The translation of the other collider.
	Vector2D otherTranslation = otherTransform->GetTranslation();

	// The other collider as a ColliderRectangle.
	const ColliderRectangle& otherRectangle = static_cast<const ColliderRectangle&>(other);

	// The bounding box of the other collider.
	BoundingRectangle otherBounding = BoundingRectangle(otherTranslation, otherRectangle.GetExtents());

	// Which sides are currently colliding.
	MapCollision mapCollision = MapCollision(
		IsSideColliding(otherBounding, RectangleSide::SideBottom),
		IsSideColliding(otherBounding, RectangleSide::SideTop),
		IsSideColliding(otherBounding, RectangleSide::SideLeft),
		IsSideColliding(otherBounding, RectangleSide::SideRight)
	);

	// If resolution is not possible, simply check if anything is colliding and return.
	if (!shouldResolve)
	{
		if (mapCollision.bottom || mapCollision.top || mapCollision.left || mapCollision.right)
		{
			// Call the map collision event handler if it exists.
			MapCollisionEventHandler collisionEventHandler = other.GetMapCollisionHandler();
			if (collisionEventHandler != nullptr)
				collisionEventHandler(*other.GetOwner(), mapCollision);

			return true;
		}
		else
		{
			return false;
		}
	}

	// The old translation of the other collider.
	Vector2D otherOldTranslation = otherPhysics->GetOldTranslation();

	Vector2D otherNewTranslation = otherTranslation;

	// The velocity of the other object.
	Vector2D otherVelocity = otherPhysics->GetVelocity();

	Vector2D otherNewVelocity = otherVelocity;

	// If nothing is colliding, return false.
	if (!(mapCollision.bottom || mapCollision.top || mapCollision.left || mapCollision.right))
		return false;

	// Resolve collisions.
	ResolveCollisions(otherBounding, otherTransform, otherPhysics, mapCollision);

	// Call the map collision event handler if it exists.
	MapCollisionEventHandler collisionEventHandler = other.GetMapCollisionHandler();
	if (collisionEventHandler != nullptr)
		collisionEventHandler(*other.GetOwner(), mapCollision);

	return true;
}

// Sets the tilemap to use for this collider.
// Params:
//   map = A pointer to the tilemap resource.
void ColliderTilemap::SetTilemap(const Tilemap* map_)
{
	map = map_;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Checks whether the specified side of a given rectangle is colliding with the tilemap.
// Params:
//   rectangle = The bounding rectangle for an object.
//   side = The side that needs to be tested for collision.
// Returns:
//   True if there is a collision, false otherwise.
bool ColliderTilemap::IsSideColliding(const BoundingRectangle& rectangle, RectangleSide side) const
{
	// % away from the center of the edge that the hotspot is at.
	static const float pointScale = 0.6f;
	// The number of units the collider's size is decreased by.
	static const float shrinkAmount = 0.01f;
	// The number of hotspots.
	static const unsigned hotspotsCount = 2;

	Vector2D hotspots[hotspotsCount];

	// Calculate hotpoint positions.
	switch (side)
	{
	case RectangleSide::SideBottom:
		hotspots[0] = Vector2D(rectangle.center.x - rectangle.extents.x * pointScale, rectangle.bottom + shrinkAmount);
		hotspots[1] = Vector2D(rectangle.center.x + rectangle.extents.x * pointScale, rectangle.bottom + shrinkAmount);
		break;
	case RectangleSide::SideTop:
		hotspots[0] = Vector2D(rectangle.center.x - rectangle.extents.x * pointScale, rectangle.top - shrinkAmount);
		hotspots[1] = Vector2D(rectangle.center.x + rectangle.extents.x * pointScale, rectangle.top - shrinkAmount);
		break;
	case RectangleSide::SideLeft:
		hotspots[0] = Vector2D(rectangle.left + shrinkAmount, rectangle.center.y - rectangle.extents.y * pointScale);
		hotspots[1] = Vector2D(rectangle.left + shrinkAmount, rectangle.center.y + rectangle.extents.y * pointScale);
		break;
	case RectangleSide::SideRight:
		hotspots[0] = Vector2D(rectangle.right - shrinkAmount, rectangle.center.y - rectangle.extents.y * pointScale);
		hotspots[1] = Vector2D(rectangle.right - shrinkAmount, rectangle.center.y + rectangle.extents.y * pointScale);
		break;
	}

	// Check if the hotspots are colliding.
	for (unsigned i = 0; i < hotspotsCount; i++)
	{
		if (IsCollidingAtPosition(hotspots[i].x, hotspots[i].y))
			return true;
	}

	return false;
}

// Determines whether a point is within a collidable cell in the tilemap.
// Params:
//   x = The x component of the point, in world coordinates.
//   y = The y component of the point, in world coordinates.
// Returns:
//   -1 if the point is outside the map, 0 if the map is empty at that position, 
//   or a positive integer if there is a tile at that position.
bool ColliderTilemap::IsCollidingAtPosition(float x, float y) const
{
	// Convert the point from world space to tile space.
	Vector2D tileSpaceCoordinate = transform->GetInverseMatrix() * Vector2D(x, y);
	unsigned col = static_cast<unsigned>(floor(tileSpaceCoordinate.x + 0.5f));
	unsigned row = static_cast<unsigned>(floor(-tileSpaceCoordinate.y + 0.5f));

	// Check if the tile is solid.
	return map->GetCellValue(col, row) > 0;
}

// Moves an object and sets its velocity based on where it collided with the tilemap.
// Params:
//   objectRectangle = A bounding rectangle that encompasses the object.
//   objectTransform = Pointer to the object's transform component.
//   objectPhysics = Pointer to the object's physics component.
//   collisions = True/false values from map collision checks.
void ColliderTilemap::ResolveCollisions(const BoundingRectangle& objectRectangle, Transform* objectTransform,
	Physics* objectPhysics, const MapCollision& collisions) const
{
	// Get the translation and velocity from the object and store them in variables.
	Vector2D translation = objectTransform->GetTranslation();
	Vector2D velocity = objectPhysics->GetVelocity();

	// We need to determine how much to move the object.
	float nudgeAmount;

	// We will do this by finding the center of the next tile, 
	// then finding the distance from the corresponding side to that position.
	float nextTileCenter;

	if (collisions.bottom || collisions.top)
	{
		if (collisions.bottom)
		{
			// Find tile above the object's bottom side.
			nextTileCenter = GetNextTileCenter(SideBottom, objectRectangle.bottom);

			// Find distance to next tile center.
			nudgeAmount = nextTileCenter - objectRectangle.bottom;
		}
		else
		{
			// Find tile below object's top side.
			nextTileCenter = GetNextTileCenter(SideTop, objectRectangle.top);

			// Find distance to next tile center.
			nudgeAmount = nextTileCenter - objectRectangle.top;
		}

		// Nudge object up/down
		translation.y += nudgeAmount;

		// Stop velocity in y direction
		velocity.y = 0;
	}

	if (collisions.left || collisions.right)
	{
		if (collisions.left)
		{
			// Find tile to the right the object's left side.
			nextTileCenter = GetNextTileCenter(SideLeft, objectRectangle.left);

			// Find distance to next tile center.
			nudgeAmount = nextTileCenter - objectRectangle.left;
		}
		else
		{
			// Find tile to the left the object's right side.
			nextTileCenter = GetNextTileCenter(SideRight, objectRectangle.right);

			// Find distance to next tile center.
			nudgeAmount = nextTileCenter - objectRectangle.right;
		}

		// Nudge object left/right.
		translation.x += nudgeAmount;

		// Stop velocity in x direction.
		velocity.x = 0;
	}

	// Modify object's actual translation and velocity.
	objectTransform->SetTranslation(translation);
	objectPhysics->SetVelocity(velocity);
}

// Returns the center of the next tile on the x-axis or y-axis.
// Used for assisting in collision resolution on a particular side.
// Params:
//   side = Which side the collision is occurring on.
//   sidePosition = The x or y value of that side.
// Returns:
//   The center of the next tile for the given position on the given side.
float ColliderTilemap::GetNextTileCenter(RectangleSide side, float sidePosition) const
{
	// Create a world space point
	Vector2D point;

	// Create a reference variable to store the result
	float* result = &point.x;

	if (side == RectangleSide::SideBottom || side == RectangleSide::SideTop)
	{
		// Use the given position for the y-axis
		// Since we only care about one axis, the other will be 0
		point.y = sidePosition;

		// Remember which part we want to modify
		result = &point.y;
	}
	else
	{
		// Use the given position for the x-axis
		// Since we only care about one axis, the other will be 0
		point.x = sidePosition;
	}

	// Transform the world space point into tile space
	point = transform->GetInverseMatrix() * point;

	// Flip the y-axis
	point.y = -point.y;

	// Move completely into cell
	point += Vector2D(0.5, 0.5);

	// Find the next tile index (higher or lower depending on the side)
	if (side == RectangleSide::SideLeft || side == RectangleSide::SideTop)
		*result = ceil(*result);
	else
		*result = floor(*result);

	// Re-adjust object position
	point -= Vector2D(0.5, 0.5);

	// Flip the y-axis
	point.y = -point.y;

	// Transform point back into world space
	point = transform->GetMatrix() * point;

	return *result;
}

//------------------------------------------------------------------------------
