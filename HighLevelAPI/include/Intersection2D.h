//------------------------------------------------------------------------------
//
// File Name:	Intersection2D.h
// Author(s):	Jeremy Kings (j.kings), David Cohen (david.cohen), and David Wong (david.wongcascante)
// Project:		Project 7
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Shapes2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Transform;
class Physics;
class ColliderRectangle;
class ColliderConvex;

namespace CS230
{
	class Matrix2D;
}

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// Check whether a point and circle intersect.
// Params:
//  point = The point in the intersection.
//  circle = The circle in the intersection.
// Returns:
//   True if intersection, false otherwise.
bool PointCircleIntersection(const Vector2D& point, const Circle& circle);

// Check whether a point and rectangle intersect.
// Params:
//  point = The point in the intersection.
//  rect  = The rectangle in the intersection.
// Returns:
//   True if intersection, false otherwise.
bool PointRectangleIntersection(const Vector2D& point, const BoundingRectangle& rect);

// Check whether two circles intersect.
// Params:
//  circle1 = The first circle.
//  circle2 = The second circle.
// Returns:
//   True if intersection, false otherwise.
bool CircleCircleIntersection(const Circle& circle1, const Circle& circle2);

// Check whether two rectangles intersect.
// Params:
//  rect1 = The first rectangle.
//	rect2 = The second rectangle.
// Returns:
//   True if intersection, false otherwise.
bool RectangleRectangleIntersection(const BoundingRectangle& rect1, const BoundingRectangle& rect2);

// Check whether a rectangle and circle intersect.
// Params:
//  rect = The rectangle.
//  circle = The circle.
// Returns:
//   True if intersection, false otherwise.
bool RectangleCircleIntersection(const BoundingRectangle& rect, const Circle& circle);

// Checks for an intersection using the separating axis theorem.
// Params:
//   axes = The array of axes to check.
//   axesSize = How many elements are in the array of axes.
//   points1 = The array of points in the first convex hull.
//   points1Size = How many points are in the first convex hull.
//   points2 = The array of points in the second convex hull.
//   points2Size = How many points are in the second convex hull.
//   radius = A value to add to/subtract from the second convex hull's projected points, essentially a radius for each point.
// Returns:
//   True if intersection, false otherwise.
bool SATIntersection(const Vector2D* axes, size_t axesSize, const Vector2D* points1, size_t points1Size, const Vector2D* points2, size_t points2Size, float radius = 0.0f);

// Calculates the (object space) corner points of a rectangle.
// Params:
//   rect = The ColliderRectangle to calculate points for.
//   out = The output array of points. Assumed to have a size of 4.
void GetOBBCorners(const ColliderRectangle& rect, Vector2D out[4]);

// Applies a transformation matrix to an array of points.
// Params:
//   matrix = The transformation matrix.
//   points = The array of points.
//   pointsSize = How many elements are in the array.
void ApplyTransformToPoints(const CS230::Matrix2D& matrix, Vector2D* points, unsigned pointsSize);

// Check whether two oriented bounding boxes intersect.
// Params:
//  rect1 = The first rectangle.
//	rect2 = The second rectangle.
// Returns:
//   True if intersection, false otherwise.
bool OBBOBBIntersection(const ColliderRectangle& rect1, const ColliderRectangle& rect2);

// Check whether an oriented bounding box and a point intersect.
// Params:
//  rect = The OBB.
//	point = The point.
// Returns:
//   True if intersection, false otherwise.
bool OBBPointIntersection(const ColliderRectangle& rect, const Vector2D& point);

// Check whether an oriented bounding box and a circle intersect.
// Params:
//  rect = The OBB.
//	circle = The circle.
// Returns:
//   True if intersection, false otherwise.
bool OBBCircleIntersection(const ColliderRectangle& rect, const Circle& circle);

// Projects a polygon into a normal
// Params:
//	normal: The normal we are using to project the polygon
//	vertices: The vertices we are projecting into the normal
//	min: The minimum value of the polygon as a result of projecting it into the normal
//	max: The maximum value of the polygon as a result of projection it into the normal
void ProjectPolygon(const Vector2D& normal, const std::vector<Vector2D>& vertices, float& min, float& max);

// Check whether two convex polygons interact
// Params:
//	lineSegments1: The line segments of the first convex polygon
//	lineSegments2: The line segments of the second convex polygon
// Returns:
//	True if intersection, false otherwise
bool ConvexHullIntersection(const std::vector<LineSegment>& lineSegments1, const std::vector<LineSegment>& lineSegments2);

// Check whether a convex polygon interacts with a rectangle collider
//	Params:
//	convexSegments: The line segments of the convex polygon
//	extents: The extents of the rectangle
//	rectTransform: The transformation of the extents of the rectangle
// Returns:
//	True if interesction, false otherwise
bool ConvexHullToOBBIntersection(const std::vector<LineSegment>& convexSegments, const Vector2D& extents, Transform& rectTransform);

// Check whether a circle is colliding with the convex collider
//	Params:
//	convexSegments: The line segments of the convex polygon
//	convexTransform: The transforma of the convex figure
//	circle: The circle we are testing against
// Returns:
//	True if intersection, false otherwise
bool ConvexHullToCircleInteresction(const std::vector<LineSegment>& convexSegments, const Transform& convexTransform, const Circle& circle);

// Checks whether a point is inside a convex shape
// Params:
//	point: The point we are testing
//	convexSegments: The line segments of the convex polygon
// Returns:
//	True if intersection, false otherwise
bool ConvexHullToPointIntersection(const std::vector<LineSegment>& convexSegments, const Vector2D& point);

// Check whether a moving point and line intersect.
// Params:
//  staticLine   = Start and end of first line segment.
//  movingPoint = Start and end of second line segment (usually a moving point).
//  intersection = Intersection point, if any.
//  t = The t value from the intersection calculation.
// Returns:
//   True if intersection, false otherwise.
bool MovingPointLineIntersection(const LineSegment& staticLine, const LineSegment& movingPoint, Vector2D& intersection, float& t);

// Modifies object's position, velocity, and rotation using simple point-line reflection.
// Params:
//  transform = Transform of the object that is being reflected.
//  physics   = Physics of the object being reflected.
//  staticLine   = Start and end of first line segment.
//  movingPoint = Start and end of second line segment (usually a moving point).
//  intersection = Intersection point of the line and circle. 
void MovingPointLineReflection(Transform& transform, Physics& physics, const LineSegment& staticLine, const LineSegment& movingPoint, const Vector2D& intersection);

// Checks whether a point is between two parallel lines.
// Params:
//   point = The point in question.
//   firstLine = A point on the first line.
//   secondLine = A point on the second line.
//   normal = Normal vector to the two lines.
// Returns:
//   True if the point is between the two lines, false otherwise.
bool PointIsBetweenLines(const Vector2D& point, const Vector2D& firstLine, const Vector2D& secondLine, const Vector2D& normal);

// Reflects a point over a line.
// Params:
//   point = The point being reflected.
//   line = The line the point will be reflected over.
// Returns:
//   The reflected point.
Vector2D ReflectPointOverLine(const Vector2D& point, const LineSegment& line);

//------------------------------------------------------------------------------
