//------------------------------------------------------------------------------
//
// File Name:	ColliderRectangle.cpp
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

#include "ColliderRectangle.h"

// Systems
#include <DebugDraw.h>
#include <Graphics.h>
#include "Intersection2D.h"
#include "GameObject.h"
#include "Parser.h"

// Components
#include "Transform.h"
#include "ColliderCircle.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for rectangle collider.
// Params:
//   extents = The rectangle's extents (half-width, half-height).
ColliderRectangle::ColliderRectangle(const Vector2D& extents) : Collider(ColliderTypeRectangle), extents(extents)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderRectangle::Clone() const
{
	return new ColliderRectangle(*this);
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file.
void ColliderRectangle::Deserialize(Parser& parser)
{
	parser.ReadVariable("extents", extents);
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file.
void ColliderRectangle::Serialize(Parser& parser) const
{
	parser.WriteVariable("extents", extents);
}

// Debug drawing for colliders.
void ColliderRectangle::Draw()
{
	// Draw the rectangle using DebugDraw.
	DebugDraw& debugDraw = DebugDraw::GetInstance();
	debugDraw.AddRectangle(transform->GetTranslation(), extents, Graphics::GetInstance().GetCurrentCamera(), Colors::Green);
}

// Get the rectangle collider's extents (half-width, half-height).
// Returns:
//	 The extents of the rectangle.
const Vector2D& ColliderRectangle::GetExtents() const
{
	return extents;
}

// Set the rectangle collider's extents (half-width, half-height).
// Params:
//   extents = The new extents of the rectangle.
void ColliderRectangle::SetExtents(const Vector2D& extents_)
{
	extents = extents_;
}

// Check for collision between a rectangle and another arbitrary collider.
// Params:
//	 other = Reference to the second collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderRectangle::IsCollidingWith(const Collider& other) const
{
	// The translation of the other collider.
	Vector2D otherTranslation = static_cast<Transform*>(other.GetOwner()->GetComponent<Transform>())->GetTranslation();

	// The BoundingRectangle for this collider.
	BoundingRectangle rectangle = BoundingRectangle(transform->GetTranslation(), extents);

	switch (other.GetType())
	{
	case ColliderTypePoint:
	{
		// Check if the point is intersecting the rectangle.
		return PointRectangleIntersection(otherTranslation, rectangle);
	}
	case ColliderTypeCircle:
	{
		// Interpret the other collider as a circle collider for ease of access.
		const ColliderCircle& otherCircle = static_cast<const ColliderCircle&>(other);

		// Check if the circle is intersecting the rectangle.
		return RectangleCircleIntersection(rectangle, Circle(otherTranslation, otherCircle.GetRadius()));
	}
	case ColliderTypeRectangle:
	{
		// Interpret the other collider as a rectangle collider for ease of access.
		const ColliderRectangle& otherRectangle = static_cast<const ColliderRectangle&>(other);

		// Check if the other rectangle is intersecting the rectangle.
		return RectangleRectangleIntersection(rectangle, BoundingRectangle(otherTranslation, otherRectangle.GetExtents()));
	}
	}

	return other.IsCollidingWith(*this);
}

//------------------------------------------------------------------------------
