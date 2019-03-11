//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.cpp
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

#include "ColliderLine.h"

// Systems
#include <Vector2D.h>
#include <DebugDraw.h>
#include <Graphics.h>
#include "Parser.h"
#include "Intersection2D.h"

// Components
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new (line) collider component.
// Params:
//   reflection = Whether points should be reflected off this collider.
ColliderLine::ColliderLine(bool reflection) : maxResolutions(4), Collider(ColliderTypeLines), reflection(reflection)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderLine::Clone() const
{
	return new ColliderLine(*this);
}

// Debug drawing for colliders.
void ColliderLine::Draw()
{
	DebugDraw& debugDraw = DebugDraw::GetInstance();

	// Add every line segment on this collider to the current line strip.
	for (unsigned i = 0; i < lineSegments.size(); i++)
	{
		// Apply the transform to the line.
		LineSegment transformedSegment = GetLineWithTransform(i);
		debugDraw.AddLineToStrip(transformedSegment.start, transformedSegment.end, Colors::Green);
		Vector2D midpoint = transformedSegment.start.Midpoint(transformedSegment.end);
		debugDraw.AddLineToStrip(midpoint, midpoint + transformedSegment.normal * 20.0f, Colors::Red);
	}
	
	// Draw the line strip.
	debugDraw.EndLineStrip(Graphics::GetInstance().GetCurrentCamera());
}

// Add a line segment to the line collider's line segment list.
// Params:
//	 p0 = The line segment's starting position.
//	 p1 = The line segment's ending position.
void ColliderLine::AddLineSegment(const Vector2D& p0, const Vector2D& p1)
{
	lineSegments.push_back(LineSegment(p0, p1));
}

namespace
{
	// Helper struct for containing intersection data.
	struct IntersectionData
	{
		// Constructor
		// Params:
		//   index = The index of the line segment this intersection data is for.
		//   intersection = The actual intersection point.
		//   t = How far into the moving point's movement the collision occured.
		//   intersected = Whether an intersection occurred.
		IntersectionData(unsigned index = 0, Vector2D intersection = Vector2D(), float t = 0.0f, bool intersected = false) : index(index), intersection(intersection), t(t), intersected(intersected)
		{
		}

		// The index of the line segment this intersection data is for.
		int index;

		// The actual intersection point.
		Vector2D intersection;

		// How far into the moving point's movement the collision occured.
		float t;

		// Whether an intersection occurred.
		bool intersected;
	};
}

// Check for collision between a line collider and another arbitrary collider.
// Params:
//	 other = Reference to the other collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderLine::IsCollidingWith(const Collider& other) const
{
	// Only handle points and circles.
	switch (other.GetType())
	{
	case ColliderTypePoint:
	case ColliderTypeCircle:
	{
		// If the line collider has no lines, nothing can collide with it.
		if (lineSegments.size() == 0)
			return false;

		int resolutionCount = 0;

		IntersectionData* intersections = nullptr;

		// If we have reflections, we're gonna need a list of intersections.
		if (reflection)
			intersections = new IntersectionData[lineSegments.size()];

		// This is in a loop to handle multiple collisions & resolutions in a single update.
		do
		{
			// Create the current moving point's line segment.
			LineSegment movingPoint(other.physics->GetOldTranslation(), other.transform->GetTranslation());

			// Iterate over every line segment and check collisions on each.
			for (unsigned i = 0; i < lineSegments.size(); i++)
			{
				Vector2D intersection;
				float t;

				// Clear the current intersection data for this line segment.
				if (reflection)
					intersections[i] = { i, Vector2D(), 0.0f, false };

				// If the lines are not intersecting, skip this line segment.
				if (!MovingPointLineIntersection(GetLineWithTransform(i), movingPoint, intersection, t))
					continue;

				if (reflection)
					// Save the current intersction data for this line segment
					intersections[i] = { i, intersection, t, true };
				else
					// If reflection is not enabled, we do not need to handle any resolutions, the event was enough.
					return true;
			}

			// Find the first intersection, the one with the lowest t.
			// Defaults to the first element, though if `intersected` is false it'll just ignore it.
			IntersectionData& firstIntersection = intersections[0];
			for (unsigned i = 0; i < lineSegments.size(); i++)
			{
				if (intersections[i].intersected && (intersections[i].t < firstIntersection.t || !firstIntersection.intersected))
					firstIntersection = intersections[i];
			}

			// Handle reflection with the first intersection, if there was one.
			if (firstIntersection.intersected)
			{
				MovingPointLineReflection(*other.transform, *other.physics, GetLineWithTransform(firstIntersection.index), movingPoint, firstIntersection.intersection);
				++resolutionCount;
			}
			else
			{
				// There were no intersections, so there's no point doing another loop.
				break;
			}
		} while (resolutionCount < maxResolutions); // Limit the number of resolutions for performance reasons.

		// Delete the dynamically allocated intersections.
		delete[] intersections;

		return resolutionCount > 0;
	}
	}

	return false;
}

// Save object data to file.
// Params:
//   parser = The parser object used to save the object's data.
void ColliderLine::Serialize(Parser& parser) const
{
	parser.WriteVariable("lineCount", lineSegments.size());
	parser.WriteValue("lines : ");
	parser.BeginScope();

	for (auto it = lineSegments.begin(); it != lineSegments.end(); it++)
		parser.WriteValue(*it);

	parser.EndScope();
}

// Load object data from file
// Params:
//   parser = The parser object used to load the object's data.
void ColliderLine::Deserialize(Parser & parser)
{
	size_t lineCount;
	parser.ReadVariable("lineCount", lineCount);
	lineSegments.reserve(lineCount);

	parser.ReadSkip("lines");
	parser.ReadSkip(':');
	parser.ReadSkip("{");

	for (size_t i = 0; i < lineCount; i++)
	{
		LineSegment lineSegment;
		parser.ReadValue(lineSegment);
		lineSegments.push_back(lineSegment);
	}
	parser.ReadSkip("}");
}

// Gets a line segment that incorporates the transform of the object
// Params:
//   index = The index of the line within the array of line segments.
LineSegment ColliderLine::GetLineWithTransform(unsigned index) const
{
	LineSegment transformedSegment = lineSegments[index];
	transformedSegment.start = transform->GetMatrix() * transformedSegment.start;
	transformedSegment.end = transform->GetMatrix() * transformedSegment.end;
	return transformedSegment;
}

// sage was here :)

//------------------------------------------------------------------------------
