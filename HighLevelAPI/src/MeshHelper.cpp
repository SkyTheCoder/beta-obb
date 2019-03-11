//------------------------------------------------------------------------------
//
// File Name:	MeshHelper.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MeshHelper.h"
#include <Mesh.h>
#include <Color.h>
#include <Vertex.h>
#include <MeshFactory.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a colored mesh consisting of a single triangle using the Beta Framework.
// Params:
//   color0 = The color of vertex 0.
//   color1 = The color of vertex 1.
//   color2 = The color of vertex 2.
// Returns:
//	 A pointer to the newly created mesh.
Mesh* CreateTriangleMesh(const Color& color0, const Color& color1, const Color& color2)
{
	// Add a triangle with the specified colors.
	MeshFactory::GetInstance().AddTriangle(
		Vertex(Vector2D(-0.5f, -0.5f), color0), // Bottom left.
		Vertex(Vector2D(0.5f, 0.0f), color1), // Middle right.
		Vertex(Vector2D(-0.5f, 0.5f), color2) // Top left.
	);

	// Finish building the mesh and return it.
	return MeshFactory::GetInstance().EndCreate();
}

// Create a textured quad mesh using the Beta Framework.
// Params:
//   textureSize = The UV size of the mesh, in texture coordinates.
//	 extents	 = The XY distance of the vertices from the origin.
// Returns:
//	 A pointer to the newly created mesh.
Mesh* CreateQuadMesh(const Vector2D& textureSize, const Vector2D& extents)
{
	// The 4 corners of the quad mesh, for ease of access.
	Vertex corners[4] = {
		Vertex(Vector2D(-extents.x, -extents.y), Vector2D(0.0f, textureSize.y)), // Bottom left.
		Vertex(Vector2D(extents.x, -extents.y), Vector2D(textureSize.x, textureSize.y)), // Bottom right.
		Vertex(Vector2D(extents.x, extents.y), Vector2D(textureSize.x, 0.0f)), // Top right.
		Vertex(Vector2D(-extents.x, extents.y), Vector2D(0.0f, 0.0f)) // Top left.
	};

	// Add the 2 triangles that make up the quad.
	MeshFactory::GetInstance().AddTriangle(corners[0], corners[2], corners[3]);
	MeshFactory::GetInstance().AddTriangle(corners[0], corners[1], corners[2]);

	// Finish building the mesh and return it.
	return MeshFactory::GetInstance().EndCreate();
}

//------------------------------------------------------------------------------
