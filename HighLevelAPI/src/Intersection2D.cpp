//------------------------------------------------------------------------------
//
// File Name:	Intersection2D.cpp
// Author(s):	Davdi Cohen (david.cohen)
// Project:		Project 7
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <stdafx.h>

#include <Intersection2D.h>

// Components
#include <Transform.h>
#include <Physics.h>
#include <ColliderConvex.cpp>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// Check whether a point and circle intersect.
// Params:
//  point = The point in the intersection.
//  circle = The circle in the intersection.
// Returns:
//   True if intersection, false otherwise.
bool PointCircleIntersection(const Vector2D& point, const Circle& circle)
{
	return point.DistanceSquared(circle.center) <= circle.radius * circle.radius;
}

// Check whether a point and rectangle intersect.
// Params:
//  point = The point in the intersection.
//  rect  = The rectangle in the intersection.
// Returns:
//   True if intersection, false otherwise.
bool PointRectangleIntersection(const Vector2D& point, const BoundingRectangle& rect)
{
	return point.x >= rect.left && point.x <= rect.right && point.y >= rect.bottom && point.y <= rect.top;
}

// Check whether two circles intersect.
// Params:
//  circle1 = The first circle.
//  circle2 = The second circle.
// Returns:
//   True if intersection, false otherwise.
bool CircleCircleIntersection(const Circle& circle1, const Circle& circle2)
{
	// Calculate the sum of all radii.
	float radiusSum = circle1.radius + circle2.radius;

	// Check if the two circles are close enough to be intersecting.
	return circle1.center.DistanceSquared(circle2.center) <= radiusSum * radiusSum;
}

// Check whether two rectangles intersect.
// Params:
//  rect1 = The first rectangle.
//	rect2 = The second rectangle.
// Returns:
//   True if intersection, false otherwise.
bool RectangleRectangleIntersection(const BoundingRectangle& rect1, const BoundingRectangle& rect2)
{
	return rect1.right >= rect2.left && rect1.left <= rect2.right && rect1.top >= rect2.bottom && rect1.bottom <= rect2.top;
}

// Check whether a rectangle and circle intersect.
// Params:
//  rect = The rectangle.
//  circle  = The circle.
// Returns:
//   True if intersection, false otherwise.
bool RectangleCircleIntersection(const BoundingRectangle& rect, const Circle& circle)
{
	// Calculate the point on the rectangle closest to the circle.
	Vector2D point = circle.center;

	if (point.x < rect.left)
		point.x = rect.left;
	if (point.x > rect.right)
		point.x = rect.right;
	if (point.y < rect.bottom)
		point.y = rect.bottom;
	if (point.y > rect.top)
		point.y = rect.top;

	// Check if the point is close enough to be intersecting the circle.
	return point.DistanceSquared(circle.center) <= circle.radius * circle.radius;
}

// Check whether two oriented bounding boxes intersect.
// Params:
//  rect1 = The first rectangle.
//	rect2 = The second rectangle.
// Returns:
//   True if intersection, false otherwise.
bool OrientedBoundingBoxIntersection(const ColliderRectangle& rect1, const ColliderRectangle& rect2)
{
	UNREFERENCED_PARAMETER(rect1);
	UNREFERENCED_PARAMETER(rect2);
	return false;
}

// Projects a polygon into a normal
// Params:
//	normal: The normal we are using to project the polygon
//	vertices: The vertices we are projecting into the normal
//	min: The minimum value of the polygon as a result of projecting it into the normal
//	max: The maximum value of the polygon as a result of projection it into the normal
void ProjectPolygon(const Vector2D& normal, const std::vector<Vector2D>& vertices, float& minValue, float& maxValue)
{
	// Get the minimum and maximum projetions of the first polygon in the line
	float minValue = -std::numeric_limits<float>::lowest();
	float maxValue = std::numeric_limits<float>::lowest();
	// Project the vertex into the normal and save the edges of the first projecte polygon into the line
	// minimum and maximum
	for (auto vertex = vertices.cbegin(); vertex < vertices.cend(); ++vertex)
	{
		float projection = vertex->DotProduct(normal);
		minValue = min(minValue, projection);
		maxValue = max(maxValue, projection);
	}
}

// Check whether two convex polygons interact
// Params:
//	polygon1: The first convex polygon
//	polygon2: The second convex polygon
// Returns:
//	True if intersection, false otherwise
bool ConvexHullIntersection(const ColliderConvex& polygon1, const ColliderConvex& polygon2)
{
	// 1.) Get the list of line segments from each polygon
	const std::vector<LineSegment> lineSegments1 = polygon1.GetLineSegments();
	const std::vector<LineSegment> lineSegments2 = polygon2.GetLineSegments();

	// Get the minimum and maximum projetions of the first polygon in the line
	float set1Min;
	float set1Max;
	// Get the minimum and maximum projections of the second polygon in the line
	float set2Min;
	float set2Max;

	// 2.) Save the vertices of both line segments
	std::vector<Vector2D> vertexSet1;
	std::vector<Vector2D> vertexSet2;
	// Save the first set of vertices
	vertexSet1.reserve(lineSegments1.size());
	vertexSet1.push_back(lineSegments1.begin()->start);
	for (auto begin = lineSegments1.cbegin(); begin < lineSegments1.cend(); ++begin)
	{
		vertexSet1.push_back(begin->end);
	}
	// Save the second set of vertices
	vertexSet2.reserve(lineSegments2.size());
	vertexSet2.push_back(lineSegments2.begin()->start);
	for (auto begin = lineSegments2.cbegin(); begin < lineSegments2.cend(); ++begin)
	{
		vertexSet2.push_back(begin->end);
	}

	// 3.) Start projecting the vertices of both polygons of into one set of axes
	for (auto begin = lineSegments1.cbegin(); begin < lineSegments1.cend(); ++begin)
	{
		// The normal from the current line segment
		Vector2D normal = begin->normal;
		ProjectPolygon(normal, vertexSet1, set1Min, set1Max);
		ProjectPolygon(normal, vertexSet2, set2Min, set2Max);
		//If there is a gap in the projection of the vertices on the axis, then stop using this set of axes as we
		// found that there is no collision; return false
		if (set1Max < set2Min || set2Max > set1Min)
		{
			return false;
		}
	}

	// 4.) Start projection the vertices of both polygons into the other set of axes
	for (auto begin = lineSegments2.cbegin(); begin < lineSegments2.cend(); ++begin)
	{
		// The normal from the current line segment
		Vector2D normal = begin->normal;
		ProjectPolygon(normal, vertexSet1, set1Min, set1Max);
		ProjectPolygon(normal, vertexSet2, set2Min, set2Max);
		//If there is a gap in the projection of the vertices on the axis, then stop using this set of axes as we
		// found that there is no collision; return false
		if (set1Max < set2Min || set2Max < set1Min)
		{
			return false;
		}
	}
	// 5.) There are no other conditions to be met, there is a collision
	return true;
}

// Check whether a convex polygon interacts with a rectangle collider
//	Params:
//	polygon: The convex polygon
//	rect: The rectangle
bool ConvexHullToOrientedBoundingBoxIntersection(const ColliderConvex& polygon, const ColliderRectangle& rect)
{
	UNREFERENCED_PARAMETER(polygon);
	UNREFERENCED_PARAMETER(rect);
	return false;
}

// Check whether a moving point and line intersect.
// Params:
//  staticLine   = Start and end of first line segment.
//  movingPoint = Start and end positions of the point, represented as a line segment.
//  intersection = Intersection point, if any.
//  t = The t value for the intersection.
// Returns:
//   True if intersection, false otherwise.
bool MovingPointLineIntersection(const LineSegment& staticLine, const LineSegment& movingPoint, Vector2D& intersection, float& t)
{
	// 1. Calculate the second object's change in translation during the current frame. Store this in your velocity variable.
	Vector2D velocity = movingPoint.end - movingPoint.start;

	// 2. Return false if the second object is stationary or is moving parallel to the line segment. 
	// (HINT: Use dot product with the velocity and line's normal!)
	if (AlmostEqual(velocity.MagnitudeSquared(), 0.0f) || AlmostEqual(abs(velocity.DotProduct(staticLine.normal)), 0.0f))
		return false;

	// 3. Using the above information, solve for t.
	t = (staticLine.normal.DotProduct(staticLine.start) - staticLine.normal.DotProduct(movingPoint.start)) / staticLine.normal.DotProduct(velocity);

	// 4. Check if intersection is between moving point start and end (if t is < 0 or > 1)
	// If not between start and end, return false
	if (t < 0.0f || t > 1.0f)
		return false;

	// 5. Calculate the point of intersection using the start, velocity, and t.
	intersection = movingPoint.start + velocity * t;

	// 6. Verify intersection point is on static segment (using static line direction as normal)
	// If intersection point is not between static line start and static line end
	//	return false
	if (!PointIsBetweenLines(intersection, staticLine.start, staticLine.end, staticLine.direction))
		return false;

	// 7. Other possibilities have been eliminated, so
	// this must be an intersection. Return true!
	return true;
}

// Checks whether a point is between two parallel lines.
// Params:
//   point = The point in question.
//   firstLine = A point on the first line.
//   secondLine = A point on the second line.
//   normal = Normal vector to the two lines.
// Returns:
//   True if the point is between the two lines, false otherwise.
bool PointIsBetweenLines(const Vector2D& point, const Vector2D& firstLine, const Vector2D& secondLine, const Vector2D& normal)
{
	// 1. Calculate distances between the line through each point and the origin, 
	// store each result in its own variable.
	// (Distance from origin to line through any point = normal dot point)
	float pointDistance = normal.DotProduct(point);
	float firstLineDistance = normal.DotProduct(firstLine);
	float secondLineDistance = normal.DotProduct(secondLine);

	// 2. If the distance to the line through "point" is less than both the others,
	// it is not between them. Return false.
	if (pointDistance < firstLineDistance && pointDistance < secondLineDistance)
		return false;

	// 3. If the distance to the line through "point" is greater than both the others,
	// it is not between them. Return false.
	if (pointDistance > firstLineDistance && pointDistance > secondLineDistance)
		return false;

	// 4. All other cases eliminated, so it must be between them. Return true!
	return true;
}

// Modifies object's position, velocity, and rotation using simple point-line reflection.
// Params:
//  transform = Transform of the object that is being reflected.
//  physics   = Physics of the object being reflected.
//  staticLine   = Start and end of first line segment.
//  movingCircle = Start and end of second line segment (can be moving point or circle).
//  intersection = Intersection point of the line and circle. 
void MovingPointLineReflection(Transform& transform, Physics& physics, const LineSegment& staticLine, const LineSegment& movingPoint, const Vector2D& intersection)
{
	// 1. Find correct position of object by reflecting its end point over the line.
	// (Use the ReflectPointOverLine function!)
	// Once found, set the object's translation to this reflected point.
	transform.SetTranslation(ReflectPointOverLine(movingPoint.end, staticLine));

	// 2. Use the reflected position and intersection point to find the
	// direction of the reflected velocity. Make sure to normalize this vector!
	Vector2D direction = (transform.GetTranslation() - intersection).Normalized();

	// Update the old translation so that multiple resolutions are handled properly.
	// direction * 0.01f fudges the translation away from the line it hit, without it, floating point error could occur during the next resolution.
	physics.SetOldTranslation(intersection + direction * 0.01f);

	// 3. Set the velocity of the object to this direction times
	// the magnitude of the current velocity.
	physics.SetVelocity(direction * physics.GetVelocity().Magnitude());

	// 4. Find the object's new rotation by using atan2 with the reflected direction.
	// Once found, set the rotation of the object to this angle.
	transform.SetRotation(atan2(direction.y, direction.x));
}

// Reflects a point over a line.
// Params:
//   point = The point being reflected.
//   line = The line the point will be reflected over.
// Returns:
//   The reflected point.
Vector2D ReflectPointOverLine(const Vector2D& point, const LineSegment& line)
{
	// 1. Pretend everything is at the origin by subtracting one of the line's 
	// points from the point we are reflecting.
	Vector2D relativePoint = point - line.start;

	// 2. Calculate the reflected point and store it in a variable.
	// Reflected point = P - 2 * (P dot n) * n
	Vector2D reflectedPoint = relativePoint - 2 * relativePoint.DotProduct(line.normal) * line.normal;

	// 3. We need to remember to move things back away from the origin before returning.
	// Return the reflected point + line.start (or line.end, if you used that at the beginning)
	return line.start + reflectedPoint;
}

//------------------------------------------------------------------------------
