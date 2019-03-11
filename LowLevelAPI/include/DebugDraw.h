//------------------------------------------------------------------------------
//
// File Name:	DebugDraw.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaEngine
// Course:		VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <vector>

#include "Shapes2D.h"
#include "Color.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Vector2D;
class Mesh;
class Camera;

//------------------------------------------------------------------------------
// Class Definition:
//------------------------------------------------------------------------------

class DebugDraw
{
public:
	//------------------------------------------------------------------------------
	// Public Functions
	//------------------------------------------------------------------------------

	// Creates debug draw meshes
	void Initialize();

	// Draws all debug objects. Should only be called by existing Low-Level API systems.
	void Draw();

	// Adds a line to the list of lines to be drawn. Note that actual drawing does not occur 
	// until EndLineStrip is called.
	// Params:
	//   start = The start point of the line segment.
	//   end   = The end point of the line segment.
	//   color = The color of the line. Defaults to white.
	void AddLineToStrip(const Vector2D& start, const Vector2D& end, const Color& color = Colors::White);
	
	// Ends the line strip formed using previous calls to AddLineToStrip.
	// Params:
	//   camera = The camera used to determine where debug objects will be drawn.
	void EndLineStrip(Camera& camera);

	// Adds a line to the list of lines to be drawn. Note that actual drawing does not occur 
	// until EndLineStrip is called.
	// Params:
	//   center = The center point of the circle.
	//   radius = The radius of the circle.
	//   camera = The camera used to determine where debug objects will be drawn.
	//   color  = The color of the circle. Defaults to white.
	void AddCircle(const Vector2D& center, float radius, Camera& camera, const Color& color = Colors::White);
	
	// Adds a line to the list of lines to be drawn. Note that actual drawing does not occur 
	// until EndLineStrip is called.
	// Params:
	//   center  = The center point of the rectangle.
	//   extents = The extents (half-width, half-height) of the rectangle.
	//   camera  = The camera used to determine where debug objects will be drawn.
	//   color   = The color of the circle. Defaults to white.
	void AddRectangle(const Vector2D& center, const Vector2D& extents, Camera& camera, const Color& color = Colors::White);

	// Enables or disables debug drawing. 
	// Note that debug drawing is only possible in debug mode.
	void SetEnabled(bool value);

	// Returns a boolean value that indicates whether debug drawing is enabled.
	bool IsEnabled() const;

	// Returns the single instance of the DebugDraw singleton.
	static DebugDraw& GetInstance();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Private to prevent accidental instantiation/destruction
	DebugDraw();
	~DebugDraw();

	DebugDraw& operator=(const DebugDraw& other) = delete;
	DebugDraw(const DebugDraw& other) = delete;

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	struct DebugCircle
	{
		DebugCircle(const Vector2D& center, float radius, const Color& color, Camera& camera);

		Circle shape; // Shape info

		Color color; // Color for drawing

		Camera& camera; // Camera to use for drawing
	};

	struct DebugRectangle
	{
		DebugRectangle(const Vector2D& center, const Vector2D& extents, const Color& color, Camera& camera);

		BoundingRectangle shape; // Shape info

		Color color; // Color for drawing

		Camera& camera; // Camera to use for drawing
	};

	struct DebugLineSegment
	{
		DebugLineSegment(const Vector2D& start, const Vector2D& end, const Color& color);

		Vector2D start; // First point of line segment
		Vector2D end;   // Second point of line segment

		Color color; // Color of the segment
	};

	struct DebugLineStrip
	{
		DebugLineStrip(const std::vector<DebugLineSegment>& segments, Camera& camera);
		~DebugLineStrip();

		Mesh* mesh;     // The mesh used when drawing this line strip
		Camera& camera; // Camera to use for drawing
	};

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Arrays of debug draw objects
	std::vector<DebugRectangle> rectangles;
	std::vector<DebugCircle> circles;
	std::vector<DebugLineStrip*> lineStrips;

	// Used when constructing line strips.
	std::vector<DebugLineSegment> lines; 

	// Meshes to use for rectangles and circles
	Mesh* circleMesh;
	Mesh* rectangleMesh;

	// Enables/disables debug drawing
	bool enabled;
};

//------------------------------------------------------------------------------
