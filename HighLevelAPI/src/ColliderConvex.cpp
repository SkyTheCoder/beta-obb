//------------------------------------------------------------------------------
//
// File Name:	ColliderConvex.cpp
// Author(s):	David Wong (david.wongcascante)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdafx.h>
#include <ColliderConvex.h>

#include <Shapes2D.h>					// 2D Shapes
#include <Intersection2D.h>				// Intersection
#include <ColliderRectangle.h>			// Rectangle Collider
#include <Transform.h>					// Transform
#include <Vector2D.h>					// Vector2D
#include <Parser.h>						// Parser
#include <GameObject.h>					// Game Object

#include <DebugDraw.h>					// Debug Draw
#include <Graphics.h>					// Graphics

//------------------------------------------------------------------------------
// Public Class Member Functions
//------------------------------------------------------------------------------
ColliderConvex::ColliderConvex(const std::vector<LineSegment>& localPoints_)
	: Collider(ColliderType::ColliderTypeConvex), localPoints()
{
	// Save the vertices of the collider
	localPoints.reserve(localPoints_.size());
	for (auto begin = localPoints_.cbegin(); begin < localPoints_.cend(); ++begin)
	{
		localPoints.push_back(*begin);
	}
}

ColliderConvex::ColliderConvex(const LineSegment* localPoints_, size_t size)
	: Collider(ColliderType::ColliderTypeConvex), localPoints()
{
	// Save the vertices of the collider
	localPoints.reserve(size);
	for (unsigned i = 0; i < size; ++i)
	{
		localPoints.push_back(localPoints_[i]);
	}
}

Component* ColliderConvex::Clone() const
{
	return new ColliderConvex(*this);
}

void ColliderConvex::Serialize(Parser& parser) const
{
	UNREFERENCED_PARAMETER(parser);
}

void ColliderConvex::Deserialize(Parser& parser)
{
	UNREFERENCED_PARAMETER(parser);
}

void ColliderConvex::Draw()
{
	// Get the debug draw
	DebugDraw& debug = DebugDraw::GetInstance();
	Graphics& graphics = Graphics::GetInstance();
	// Iterate through every point in the convex shape
	auto begin = localPoints.cbegin();
	for (; begin < localPoints.cend(); ++begin)
	{
		// Draw a line between the current point and the next in green
		debug.AddLineToStrip(begin->start, begin->end, Colors::Green);
		// Draw the normal of the line segment in blue
		Vector2D midpoint = begin->start.Midpoint(begin->end);
		debug.AddLineToStrip(midpoint, midpoint + begin->normal * 20, Colors::Blue);
	}
	// Draw a line between the last point in the convex shape and the first in green
	// End the line strip so it shows up
	debug.EndLineStrip(graphics.GetCurrentCamera());
}

bool ColliderConvex::IsCollidingWith(const Collider& other) const
{
	// Only bother colliding with with other convex colliders and rectangles
	switch (other.GetType())
	{
	case ColliderType::ColliderTypeConvex:
	{
		const ColliderConvex& convex = static_cast<const ColliderConvex&>(other);
		return ConvexHullIntersection(*this, convex);
	}
	case ColliderType::ColliderTypeRectangle:
	{
		const ColliderRectangle& rectangle = static_cast<const ColliderRectangle&>(other);
		return ConvexHullToOrientedBoundingBoxIntersection(*this, rectangle);

	}
	default:
		return false;
	}
}

const std::vector<LineSegment>& ColliderConvex::GetLocalLineSegments() const
{
	return localPoints;
}

std::vector<LineSegment> ColliderConvex::GetLineSegments() const
{
	Transform* transform = GetOwner()->GetComponent<Transform>();
	std::vector<LineSegment> transformed;
	transformed.reserve(localPoints.size());

	for (auto begin = localPoints.cbegin(); begin < localPoints.cend(); ++begin)
	{
		LineSegment transformedSegment(transform->GetMatrix() * begin->start, transform->GetMatrix() * begin->end);
		transformed.push_back(transformedSegment);
	}

	return transformed;
}