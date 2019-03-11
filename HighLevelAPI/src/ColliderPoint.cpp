//------------------------------------------------------------------------------
//
// File Name:	ColliderPoint.cpp
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

#include "ColliderPoint.h"

// Systems
#include <DebugDraw.h>
#include <Graphics.h>
#include "Intersection2D.h"
#include "GameObject.h"

// Components
#include "Transform.h"
#include "ColliderCircle.h"
#include "ColliderRectangle.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for point collider.
ColliderPoint::ColliderPoint() : Collider(ColliderTypePoint)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderPoint::Clone() const
{
	return new ColliderPoint(*this);
}

// Debug drawing for colliders.
void ColliderPoint::Draw()
{
	// Draw the point using DebugDraw.
	DebugDraw& debugDraw = DebugDraw::GetInstance();
	debugDraw.AddCircle(transform->GetTranslation(), 10.0f, Graphics::GetInstance().GetCurrentCamera(), Colors::Green);
}

// Check for collision between a point and another arbitrary collider.
// Params:
//	 other = Reference to the second collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderPoint::IsCollidingWith(const Collider& other) const
{
	// The translation of the other collider.
	Vector2D otherTranslation = static_cast<Transform*>(other.GetOwner()->GetComponent<Transform>())->GetTranslation();

	switch (other.GetType())
	{
	case ColliderTypePoint:
	{
		// Points cannot collide with each other.
		return false;
	}
	case ColliderTypeCircle:
	{
		// Interpret the other collider as a circle collider for ease of access.
		const ColliderCircle& otherCircle = static_cast<const ColliderCircle&>(other);

		// Check if the point is intersecting the circle.
		return PointCircleIntersection(transform->GetTranslation(), Circle(otherTranslation, otherCircle.GetRadius()));
	}
	case ColliderTypeRectangle:
	{
		const ColliderRectangle& otherRectangle = static_cast<const ColliderRectangle&>(other);

		// Check if the point is intersecting the rectangle.
		return PointRectangleIntersection(transform->GetTranslation(), BoundingRectangle(otherTranslation, otherRectangle.GetExtents()));
	}
	}

	return other.IsCollidingWith(*this);
}

//------------------------------------------------------------------------------
