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
#include <Transform.h>					// Transform
#include <Vector2D.h>					// Vector2D
#include <Parser.h>						// Parser
#include <GameObject.h>					// Game Object

#include <DebugDraw.h>					// Debug Draw
#include <Graphics.h>					// Graphics

#include <ColliderRectangle.h>			// Rectangle Collider
#include <ColliderCircle.h>				// Circle Collider
//------------------------------------------------------------------------------
// Public Class Member Functions
//------------------------------------------------------------------------------
ColliderConvex::ColliderConvex()
	: Collider(ColliderType::ColliderTypeConvex)
{

}

ColliderConvex::ColliderConvex(const std::vector<LineSegment>& localLines_)
	: Collider(ColliderType::ColliderTypeConvex), localLines()
{
	// Save the vertices of the collider
	localLines.reserve(localLines_.size());
	for (auto begin = localLines_.cbegin(); begin < localLines_.cend(); ++begin)
	{
		localLines.push_back(*begin);
	}
}

ColliderConvex::ColliderConvex(const LineSegment* localPoints_, size_t size)
	: Collider(ColliderType::ColliderTypeConvex), localLines()
{
	// Save the vertices of the collider
	localLines.reserve(size);
	for (unsigned i = 0; i < size; ++i)
	{
		localLines.push_back(localPoints_[i]);
	}
}

Component* ColliderConvex::Clone() const
{
	return new ColliderConvex(*this);
}

void ColliderConvex::Serialize(Parser& parser) const
{
	// Write the number of line segments that we have
	parser.WriteVariable("lineCount", localLines.size());
	// Then write the lines we've got
	parser.WriteVariable("lines", "");
	parser.BeginScope();
	for (auto begin = localLines.cbegin(); begin < localLines.cend(); begin++)
	{
		parser.WriteValue(*begin);
	}
	parser.EndScope();
}

void ColliderConvex::Deserialize(Parser& parser)
{
	// Read the amont of lines we've got
	int lineCount;
	parser.ReadVariable("lineCount", lineCount);
	// Skip over the lines variable
	parser.ReadSkip("lines");
	parser.ReadSkip(':');
	parser.ReadSkip('{');
	// Read the lines
	localLines.reserve(lineCount);

	for (unsigned i = 0; i < lineCount; ++i)
	{
		LineSegment segment;
		parser.ReadValue(segment);
		localLines.push_back(segment);
	}
	// Skip over the ending scope
	parser.ReadSkip('}');
}

void ColliderConvex::Draw()
{
	// Get the debug draw
	DebugDraw& debug = DebugDraw::GetInstance();
	Graphics& graphics = Graphics::GetInstance();
	// Iterate through every point in the convex shape
	std::vector<LineSegment> transformed = GetLineSegments();
	auto begin = transformed.cbegin();
	for (; begin < transformed.cend(); ++begin)
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
		return ConvexHullIntersection(GetLineSegments(), convex.GetLineSegments());
	}
	case ColliderType::ColliderTypeRectangle:
	{
		const ColliderRectangle& rectangle = static_cast<const ColliderRectangle&>(other);
		Transform* rectTransform = other.GetOwner()->GetComponent<Transform>();
		return ConvexHullToOBBIntersection(GetLineSegments(), rectangle.GetExtents(), *rectTransform);
	}
	case ColliderType::ColliderTypeCircle:
	{
		const ColliderCircle& circle = static_cast<const ColliderCircle&>(other);
		const Transform* convexTransform = GetOwner()->GetComponent<Transform>();
		const Transform* circleTransform = other.GetOwner()->GetComponent<Transform>();
		return ConvexHullToCircleInteresction(GetLineSegments(), *convexTransform, 
			Circle(circleTransform->GetTranslation(), circle.GetRadius()));
	}
	default:
		return false;
	}
}

void ColliderConvex::AddSide(const LineSegment& segment)
{
	localLines.push_back(LineSegment(segment));
}

const std::vector<LineSegment>& ColliderConvex::GetLocalLineSegments() const
{
	return localLines;
}

std::vector<LineSegment> ColliderConvex::GetLineSegments() const
{
	std::vector<LineSegment> transformed;
	transformed.reserve(localLines.size());

	for (auto begin = localLines.cbegin(); begin < localLines.cend(); ++begin)
	{
		LineSegment transformedSegment(transform->GetMatrix() * begin->start, transform->GetMatrix() * begin->end);
		transformed.push_back(transformedSegment);
	}

	return transformed;
}