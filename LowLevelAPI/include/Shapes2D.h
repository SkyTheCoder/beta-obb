//------------------------------------------------------------------------------
//
// File Name:	Shapes2D.h
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

#include "Vector2D.h"

//------------------------------------------------------------------------------

struct LineSegment
{
	// Constructors
	LineSegment();
	LineSegment(const Vector2D& p0, const Vector2D& p1);

	// Points marking each end of a single line segment.
	Vector2D start;
	Vector2D end;

	// Direction of the line segment, precomputed from start and end.
	Vector2D direction;

	// Normal of the line segment, precomputed from start and end.
	Vector2D normal;
};

std::ostream& operator<<(std::ostream& os, const LineSegment& line);
std::istream& operator>>(std::istream& is, LineSegment& line);

struct BoundingRectangle
{
	// Constructor
	BoundingRectangle(const Vector2D& center, const Vector2D& extents);

	Vector2D center;  // Center of the rectangle.
	Vector2D extents; // HALF the width and height of the rectangle.
	float left;		  // X-coord of the left side of the rectangle
	float right;	  // X-coord of the right side of the rectangle
	float top;		  // Y-coord of the top side of the rectangle
	float bottom;	  // Y-coord of the bottom side of the rectangle
};

struct Circle
{
	// Constructor
	Circle(const Vector2D& center, float radius)
		: center(center), radius(radius) {};

	Vector2D center; // Center point of circle.
	float radius;    // Radius of circle.
};

//------------------------------------------------------------------------------
