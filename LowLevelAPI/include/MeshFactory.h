//------------------------------------------------------------------------------
//
// File Name:	MeshFactory.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
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
#include "Vector3D.h"
#include "Color.h"
#include "Mesh.h" // MDM_Triangles

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

struct Vertex;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class MeshFactory
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a textured quad mesh using the Beta Framework.
	// Params:
	//	 halfSize = The XY distance of the vertices from the origin, defaults to 0.5 on each axis.
	//   textureSize = The UV size of the mesh, relative to texture coordinates, defaults to 1.0 for each.
	// Returns:
	//	 If the memory allocation was successful, then return a pointer to the allocated memory,
	//	   else return nullptr.
	Mesh* CreateQuadMesh(const Vector2D& textureSize = Vector2D(1.0f, 1.0f), const Vector2D& halfSize = Vector2D(0.5f, 0.5f));

	// Create a colored quad mesh using the Beta Framework.
	// Params:
	//	 halfSize = The XY distance of the vertices from the origin, defaults to 0.5 on each axis.
	//	 color = The color assigned to each vertex (for meshes not using textures).
	// Returns:
	//	 If the memory allocation was successful, then return a pointer to the allocated memory,
	//	   else return nullptr.
	Mesh* CreateQuadMesh(const Color& color, const Vector2D& halfSize = Vector2D(0.5f, 0.5f));

	// Stops adding vertices and returns a dynamically allocated mesh.
	// Params:
	//   drawMode = How to interpret the vertices of the mesh (usually as triangles).
	// Returns:
	//   A mesh pointer that contains all the vertices added since StartCreate was called.  
	Mesh* EndCreate(MeshDrawMode drawMode = MDM_Triangles);

	// Add a colored vertex to the mesh being created.
	void AddVertex(const Vertex& vertex);

	// Add a textured triangle (three vertices) to the mesh being created.
	void AddTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	// Add a 2D colored triangle (three vertices) to the mesh being created.
	void AddTriangle(const Vector2D& v0, const Vector2D& v1, const Vector2D& v2, const Color& color);

	// Return an instance of the mesh factory
	static MeshFactory& GetInstance();

private:
	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------

	const int floatsPerPosition;		// number of floats per position
	const int floatsPerColor;			// number of floats per color
	const int floatsPerTextureCoord;	// number of floats per texture coordinate

	const int positionStride; // bytes per vertex position
	const int colorStride;    // bytes per vertex color
	const int textureCoordStride; // bytes per texture coordinate

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Constructor/destructor private to prevent accidental instantiation/destruction
	MeshFactory();
	~MeshFactory();

	// Copy constructor and assignment operator disabled
	MeshFactory(const MeshFactory& other) = delete;
	MeshFactory& operator=(const MeshFactory& other) = delete;

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	std::vector<Vector3D> positions;
	std::vector<Color> colors;
	std::vector<Vector2D> textureCoords;
};

//------------------------------------------------------------------------------
