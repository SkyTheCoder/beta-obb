//------------------------------------------------------------------------------
//
// File Name:	Intersection2D.cpp
// Author(s):	David Cohen (david.cohen) and David Wong (david.wongcascante)
// Project:		Project 7
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Intersection2D.h"

// Components
#include "Transform.h"
#include "Physics.h"
#include "ColliderRectangle.h"
#include "ColliderConvex.h"

// Systems
#include <DebugDraw.h>
#include <Graphics.h>
#include <Camera.h>

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

// Checks for an intersection using the separating axis theorem.
// Params:
//   axes = The array of axes to check.
//   axesSize = How many elements are in the array of axes.
//   points1 = The array of points in the first convex hull.
//   points1Size = How many points are in the first convex hull.
//   points2 = The array of points in the second convex hull.
//   points2Size = How many points are in the second convex hull.
//   points2Offset = A value to add to/subtract from the second convex hull's projected points, essentially a radius for each point.
// Returns:
//   True if intersection, false otherwise.
bool SATIntersection(const Vector2D* axes, unsigned axesSize, const Vector2D* points1, unsigned points1Size, const Vector2D* points2, unsigned points2Size, float points2Offset)
{
	DebugDraw& debugDraw = DebugDraw::GetInstance();
	Camera& currentCamera = Graphics::GetInstance().GetCurrentCamera();
	
	// Draw the axes' lines.
	for (unsigned i = 0; i < axesSize; i++)
	{
		debugDraw.AddLineToStrip(-400.0f * axes[i], 400.0f * axes[i], Colors::LightBlue);
	}
	debugDraw.EndLineStrip(currentCamera);

	// Loop through each axis.
	for (unsigned i = 0; i < axesSize; i++)
	{
		float minExtents1 = FLT_MAX;
		float maxExtents1 = -FLT_MAX;
		float minExtents2 = FLT_MAX;
		float maxExtents2 = -FLT_MAX;

		for (unsigned j = 0; j < points1Size; j++)
		{
			// Project the current point along the current axis.
			float projectedPoint = points1[j].DotProduct(axes[i]);
			
			// Draw the point being projected along the current axis.
			debugDraw.AddLineToStrip(points1[j], axes[i] * projectedPoint, Colors::Yellow);
			debugDraw.AddCircle(axes[i] * projectedPoint, 4.0f, currentCamera, Colors::Violet);

			// Update the min/max extents of each convex hull.
			maxExtents1 = max(maxExtents1, projectedPoint);
			minExtents1 = min(minExtents1, projectedPoint);
		}

		for (unsigned j = 0; j < points2Size; j++)
		{
			// Project the current point along the current axis.
			float projectedPoint = points2[j].DotProduct(axes[i]);

			// Draw the point being projected along the current axis.
			debugDraw.AddLineToStrip(points2[j] - axes[i] * points2Offset, axes[i] * (projectedPoint - points2Offset), Colors::Yellow);
			debugDraw.AddCircle(axes[i] * (projectedPoint - points2Offset), 4.0f, currentCamera, Colors::Violet);
			debugDraw.AddLineToStrip(points2[j] + axes[i] * points2Offset, axes[i] * (projectedPoint + points2Offset), Colors::Yellow);
			debugDraw.AddCircle(axes[i] * (projectedPoint + points2Offset), 4.0f, currentCamera, Colors::Violet);

			// Update the min/max extents of each convex hull.
			maxExtents2 = max(maxExtents2, projectedPoint);
			minExtents2 = min(minExtents2, projectedPoint);
		}

		debugDraw.EndLineStrip(currentCamera);

		float gap1 = (minExtents2 - points2Offset) - maxExtents1;
		float gap2 = minExtents1 - (maxExtents2 + points2Offset);
		float gap = max(gap1, gap2);

		// If there is a gap between the min and max extents on this axis, the convex hulls are not colliding.
		if (gap > 0)
		{
			if (gap1 > gap2)
			{
				debugDraw.AddLineToStrip(maxExtents1 * axes[i], (maxExtents1 + gap) * axes[i], Colors::Red);
			}
			else
			{
				debugDraw.AddLineToStrip((maxExtents2 + points2Offset) * axes[i], ((maxExtents2 + points2Offset) + gap) * axes[i], Colors::Red);
			}
			debugDraw.EndLineStrip(currentCamera);
			return false;
		}

		// If there is a gap between the min and max extents on this axis, the convex hulls are not colliding.
		//if (maxExtents1 < (minExtents2 - points2Offset) || (maxExtents2 + points2Offset) < minExtents1)
		//	return false;
	}

	// If we reached this point, there is no gap, the OBBs are colliding.
	return true;
}

// Calculates the (object space) corner points of a rectangle.
// Params:
//   rect = The ColliderRectangle to calculate points for.
//   out = The output array of points. Assumed to have a size of 4.
void GetOBBCorners(const ColliderRectangle& rect, Vector2D out[4])
{
	Vector2D extents = rect.GetExtents();
	Vector2D scale = rect.transform->GetScale();

	// These points will have the transformation matrix applied to them later, so we want to remove scaling right now.
	extents.x /= scale.x;
	extents.y /= scale.y;

	// Calculate the corners.
	out[0] = Vector2D(-extents.x, extents.y);
	out[1] = extents;
	out[2] = Vector2D(extents.x, -extents.y);
	out[3] = -extents;
}

// Applies a transformation matrix to an array of points.
// Params:
//   matrix = The transformation matrix.
//   points = The array of points.
//   pointsSize = How many elements are in the array.
void ApplyTransformToPoints(const CS230::Matrix2D& matrix, Vector2D* points, unsigned pointsSize)
{
	for (unsigned i = 0; i < pointsSize; i++)
		points[i] = matrix * points[i];
}

// Check whether two oriented bounding boxes intersect.
// Params:
//  rect1 = The first rectangle.
//	rect2 = The second rectangle.
// Returns:
//   True if intersection, false otherwise.
bool OBBOBBIntersection(const ColliderRectangle& rect1, const ColliderRectangle& rect2)
{
	float angle1 = rect1.transform->GetRotation();
	float angle2 = rect2.transform->GetRotation();

	Vector2D axes[4];

	// Calculate all the axes we need to check for gaps along.
	axes[0] = Vector2D(cos(angle1), sin(angle1));
	axes[1] = Vector2D(-axes[0].y, axes[0].x);
	axes[2] = Vector2D(cos(angle2), sin(angle2));
	axes[3] = Vector2D(-axes[2].y, axes[2].x);

	Vector2D points1[4];
	Vector2D points2[4];

	// Gather the points for both rectangles.
	GetOBBCorners(rect1, points1);
	GetOBBCorners(rect2, points2);

	// Apply the transformation matrices to the rectangles' points.
	ApplyTransformToPoints(rect1.transform->GetMatrix(), points1, 4);
	ApplyTransformToPoints(rect2.transform->GetMatrix(), points2, 4);

	// Perform the intersection test.
	return SATIntersection(axes, 4, points1, 4, points2, 4);
}

// Check whether an oriented bounding box and a point intersect.
// Params:
//  rect = The OBB.
//	point = The point.
// Returns:
//   True if intersection, false otherwise.
bool OBBPointIntersection(const ColliderRectangle& rect, const Vector2D& point)
{
	float angle1 = rect.transform->GetRotation();

	Vector2D axes[3];

	// Calculate all the axes we need to check for gaps along.
	axes[0] = Vector2D(cos(angle1), sin(angle1));
	axes[1] = Vector2D(-axes[0].y, axes[0].x);
	axes[2] = (point - rect.transform->GetTranslation()).Normalized(); // Direction from the rectangle to the point

	Vector2D points[4];

	// Gather the points for the rectangle.
	GetOBBCorners(rect, points);

	// Apply the transformation matrix to the rectangle's points.
	ApplyTransformToPoints(rect.transform->GetMatrix(), points, 4);

	// Perform the intersection test.
	return SATIntersection(axes, 3, points, 4, &point, 1);
}

// Check whether an oriented bounding box and a circle intersect.
// Params:
//  rect = The rectangle.
//	circle = The circle.
// Returns:
//   True if intersection, false otherwise.
bool OBBCircleIntersection(const ColliderRectangle& rect, const Circle& circle)
{
	float angle1 = rect.transform->GetRotation();

	Vector2D axes[3];

	// Calculate all the axes we need to check for gaps along.
	axes[0] = Vector2D(cos(angle1), sin(angle1));
	axes[1] = Vector2D(-axes[0].y, axes[0].x);
	axes[2] = (circle.center - rect.transform->GetTranslation()).Normalized(); // Direction from the rectangle to the circle

	Vector2D points[4];

	// Gather the points for the rectangle.
	GetOBBCorners(rect, points);
	
	// Apply the transformation matrix to the rectangle's points.
	ApplyTransformToPoints(rect.transform->GetMatrix(), points, 4);

	// Perform the intersection test.
	return SATIntersection(axes, 3, points, 4, &circle.center, 1, circle.radius);
}

// Projects a polygon into a normal
// Params:
//	normal: The normal we are using to project the polygon
//	vertices: The vertices we are projecting into the normal
//	min: The minimum value of the polygon as a result of projecting it into the normal
//	max: The maximum value of the polygon as a result of projection it into the normal
void ProjectPolygon(const Vector2D& normal, const std::vector<Vector2D>& vertices, float& minValue, float& maxValue)
{
	// Debug draw information for every projection
	DebugDraw& debug = DebugDraw::GetInstance();
	Camera& currentCamera = Graphics::GetInstance().GetCurrentCamera();
	// Get the minimum and maximum projetions of the first polygon in the line
	minValue = FLT_MAX;
	maxValue = FLT_MIN;
	// Draw the normal
	debug.AddLineToStrip(-normal * 800, normal * 800, Colors::Violet);
	// Project the vertex into the normal and save the edges of the first projecte polygon into the line
	// minimum and maximum
	for (auto vertex = vertices.cbegin(); vertex < vertices.cend(); ++vertex)
	{
		float projection = vertex->DotProduct(normal);
		minValue = min(minValue, projection);
		maxValue = max(maxValue, projection);
	}
	debug.AddCircle(normal * minValue, 10, currentCamera, Colors::Yellow);
	debug.AddCircle(normal * maxValue, 10, currentCamera, Colors::Yellow);
	debug.EndLineStrip(currentCamera);
	
}

// Check whether two convex polygons interact
// Params:
//	lineSegments1: The first convex polygon
//	lineSegments2: The second convex polygon
// Returns:
//	True if intersection, false otherwise
bool ConvexHullIntersection(const std::vector<LineSegment>& lineSegments1, const std::vector<LineSegment>& lineSegments2)
{
	// Get the minimum and maximum projetions of the first polygon in the line
	float set1Min;
	float set1Max;
	// Get the minimum and maximum projections of the second polygon in the line
	float set2Min;
	float set2Max;

	// 2.) Save the vertices and normals of both line segments
	std::vector<Vector2D> vertexSet1;
	std::vector<Vector2D> vertexSet2;
	std::vector<Vector2D> normalSet;
	unsigned totalSize = lineSegments1.size() + lineSegments2.size();
	vertexSet1.reserve(totalSize);
	vertexSet2.reserve(totalSize);
	normalSet.reserve(totalSize);

	for (unsigned i = 0; i < totalSize; ++i)
	{
		if (i < lineSegments1.size())
		{
			vertexSet1.push_back(lineSegments1[i].end);
			for (unsigned j = 0; j < i; ++j)
			{
				if (lineSegments1[i].normal.DotProduct(normalSet[j]) == 0)
					continue;
			}
			normalSet.push_back(lineSegments1[i].normal);
		}
		else
		{
			vertexSet2.push_back(lineSegments2[i - lineSegments1.size()].end);
			for (unsigned j = i - lineSegments1.size(); j < normalSet.size(); ++j)
			{
				if (lineSegments2[i - lineSegments1.size()].normal.DotProduct(normalSet[j]) == 0)
					continue;
			}
			normalSet.push_back(lineSegments2[i - lineSegments1.size()].normal);
		}
	}

	return SATIntersection(normalSet.data(), normalSet.size(), vertexSet1.data(), vertexSet1.size(), vertexSet2.data(), vertexSet2.size());
}

// Check whether a convex polygon interacts with a rectangle collider
//	Params:
//	convexSegments: The line segments of the convex polygon
//	extents: The extents of the rectangle
bool ConvexHullToOBBIntersection(const std::vector<LineSegment>& convexSegments, const Vector2D& extents, Transform& rectTransform)
{
	// Create the line segments so that they are rotated but not scaled
	// The horizontal end is the same as the vertical side's start
	Vector2D topLeft(-extents.x, extents.y);
	Vector2D topRight(extents);
	Vector2D bottomRight(extents.x, -extents.y);
	Vector2D bottomLeft(-extents);

	// Transform the vertices by the rotation but also scale them back
	Vector2D translation = rectTransform.GetTranslation();
	Matrix2D matrix = Matrix2D::RotationMatrixRadians(rectTransform.GetRotation()) * Matrix2D::TranslationMatrix(translation.x, translation.y);
	topLeft = matrix * topLeft;
	topRight = matrix * topRight;
	bottomRight = matrix * bottomRight;
	bottomLeft = matrix * bottomLeft;

	// Construct a convex hull out of the extents of the circle
	std::vector<LineSegment> extentSegments;
	// Because a rectangle has two pairs of parallel sides, then we only need to check for those two sides, lol.
	extentSegments.reserve(4);
	extentSegments.push_back(LineSegment(topLeft, topRight));
	extentSegments.push_back(LineSegment(topRight, bottomRight));
	extentSegments.push_back(LineSegment(bottomRight, bottomLeft));
	extentSegments.push_back(LineSegment(bottomLeft, topLeft));
	// Finally create the 
	// Then, just use the convex hull collision detection algorithm already written
	return ConvexHullIntersection(convexSegments, extentSegments);
}

// Check whether a circle is colliding with the convex collider
//	Params:
//	convexSegments: The line segments of the convex polygon
//	convexTransform: The transform of the convex figure
//	circle: The circle we are testing against
// Returns:
//	True if intersection, false otherwise
bool ConvexHullToCircleInteresction(const std::vector<LineSegment>& convexSegments, const Transform& convexTransform, const Circle& circle)
{
	// Separate our segments into vertices and normals
	std::vector<Vector2D> vertexSet;
	std::vector<Vector2D> normalSet;
	vertexSet.reserve(convexSegments.size());
	normalSet.reserve(convexSegments.size() + 3);
	for (auto begin = convexSegments.cbegin(); begin < convexSegments.cend(); ++begin)
	{
		vertexSet.push_back(begin->end);
		normalSet.push_back(begin->normal);
	}

	// Add the three normals that come from the circle
	// One from the rotation of the convex shape
	Vector2D circleTangent(cosf(convexTransform.GetRotation()), sinf(convexTransform.GetRotation()));
	normalSet.push_back(circleTangent);
	// One that is perpendicular to the tangent
	normalSet.push_back(Vector2D(-circleTangent.y, circleTangent.x));
	// One that goes from the middle of the circle to the middle of the convex
	normalSet.push_back((convexTransform.GetTranslation() - circle.center).Normalized());

	// Test the collisions
	return SATIntersection(normalSet.data(), normalSet.size(), vertexSet.data(), vertexSet.size(), &circle.center, 1, circle.radius);
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
