//------------------------------------------------------------------------------
//
// File Name:	ColliderCircle.cpp
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

#include "ColliderCircle.h"

// Systems
#include <DebugDraw.h>
#include <Graphics.h>
#include "Intersection2D.h"
#include "GameObject.h"
#include "Parser.h"

// Components
#include "Transform.h"
#include "ColliderRectangle.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for circle collider.
// Params:
//   radius = The radius of the circle.
ColliderCircle::ColliderCircle(float radius) : Collider(ColliderTypeCircle), radius(radius)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderCircle::Clone() const
{
	return new ColliderCircle(*this);
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file.
void ColliderCircle::Deserialize(Parser& parser)
{
	parser.ReadVariable("radius", radius);
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file.
void ColliderCircle::Serialize(Parser& parser) const
{
	parser.WriteVariable("radius", radius);
}

// Debug drawing for colliders.
void ColliderCircle::Draw()
{
	// Draw the circle using DebugDraw.
	DebugDraw& debugDraw = DebugDraw::GetInstance();
	debugDraw.AddCircle(transform->GetTranslation(), radius, Graphics::GetInstance().GetCurrentCamera(), Colors::Green);
}

// Get the circle collider's radius.
// Returns:
//	 The circle collider's radius.
float ColliderCircle::GetRadius() const
{
	return radius;
}

// Set the circle collider's radius.
// Params:
//   radius = The circle collider's new radius.
void ColliderCircle::SetRadius(float radius_)
{
	radius = radius_;
}

// Check for collision between a circle and another arbitrary collider.
// Params:
//	 other = Reference to the second collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderCircle::IsCollidingWith(const Collider& other) const
{
	// The translation of the other collider.
	Vector2D otherTranslation = other.GetOwner()->GetComponent<Transform>()->GetTranslation();

	// The Circle for this collider.
	Circle circle = Circle(transform->GetTranslation(), radius);

	switch (other.GetType())
	{
	case ColliderTypePoint:
	{
		// Check if the point is intersecting the circle.
		return PointCircleIntersection(otherTranslation, circle);
	}
	case ColliderTypeCircle:
	{
		// Interpret the other collider as a circle collider for ease of access.
		const ColliderCircle& otherCircle = static_cast<const ColliderCircle&>(other);

		// Check if the other circle is intersecting the circle.
		return CircleCircleIntersection(Circle(otherTranslation, otherCircle.GetRadius()), circle);
	}
	}

	return other.IsCollidingWith(*this);
}

//------------------------------------------------------------------------------
