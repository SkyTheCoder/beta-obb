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
// Include Files:
//------------------------------------------------------------------------------

#include <Collider.h>				// Collider
#include <vector>					// Vector

//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------
struct LineSegment;

//------------------------------------------------------------------------------
// Public Structures
//------------------------------------------------------------------------------

class ColliderConvex : public Collider
{
private:
	//------------------------------------------------------------------------------
	// Private Variables
	//------------------------------------------------------------------------------

	// The local points of the collider
	std::vector<LineSegment> localPoints;
public:
	//------------------------------------------------------------------------------
	// Public Functions
	//------------------------------------------------------------------------------

	// Constructs an emptpy convex polygon collider
	ColliderConvex();

	// Constructs a convex polygon colllider
	// Params:
	//	localPoints: The points of the convex polygon in relation to the center of it
	ColliderConvex(const std::vector<LineSegment>& localPoints);
	
	// Constructs a convex polygon collider
	// Params:
	//	localPoints: The points of the convex polygon in relation to its center
	//	size: The number of points we are putting in
	ColliderConvex(const LineSegment* localPoints, size_t size);

	// Clones this component
	// Returns:
	//	A clone of this component
	Component* Clone() const;

	// Serializes the component to a file
	// Params:
	//	parser: The parser we are using to write the component into a file
	virtual void Serialize(Parser& parser) const override;

	// Reads the component from a file
	// Params:
	//	parser: The parser we are using to read the component from the file
	virtual void Deserialize(Parser& parser) override;

	// Debug Draw of the Collider
	void Draw() override;
	
	// Adds a line segment to the collider, assuming it keeps the collider a convex shape
	// if the collider's shape is not a convex, then the collision detection will not be accurate
	// Params:
	//	segment: The line segment
	void AddSide(const LineSegment& segment);

	// Check for a collision between a convex polygon and another collider
	// Params:
	//	other: The other collider we are checking with
	// Returns:
	//	Whether the two colliders are colliding with each other
	bool IsCollidingWith(const Collider& with) const override;

	// Gets the line segments of the convex collider in local space
	// Returns:
	//	A list with the line segments of the line collider
	const std::vector<LineSegment>& GetLocalLineSegments() const;

	// Gets the line segments transformed to world space
	// Returns:
	//	A list with the line segments transformed to world space
	std::vector<LineSegment> GetLineSegments() const;

};