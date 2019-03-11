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
// Forward Declarations:
//------------------------------------------------------------------------------

class Vector2D;

//------------------------------------------------------------------------------
// Public Structures
//------------------------------------------------------------------------------

class ColliderConvex : public Collider
{
private:
	// The local points of the collider
	std::vector<Vector2D> localPoints;
public:
	// Constructs a convex polygon colllider
	// Params:
	//	localPoints: The points of the convex polygon in relation to the center of it
	ColliderConvex(std::vector<Vector2D> localPoints);
	
	// Constructs a convex polygon collider
	// Params:
	//	localPoints: The points of the convex polygon in relation to its center
	//	size: The number of points we are putting in
	ColliderConvex(Vector2D* localPoints, size_t size);

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

	// Check for a collision between a convex polygon and another collider
	// Params:
	//	other: The other collider we are checking with
	// Returns:
	//	Whether the two colliders are colliding with each other
	bool IsCollidingWith(const Collider& with) const override;

}