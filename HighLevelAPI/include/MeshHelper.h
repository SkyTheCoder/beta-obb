//------------------------------------------------------------------------------
//
// File Name:	MeshHelper.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Vector2D.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Mesh Mesh;
typedef class Color Color;

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
Mesh* CreateTriangleMesh(const Color& color0, const Color& color1, const Color& color2);

// Create a textured quad mesh using the Beta Framework.
// Params:
//   textureSize = The UV size of the mesh, in texture coordinates.
//	 extents	 = The XY distance of the vertices from the origin.
// Returns:
//	 A pointer to the newly created mesh.
Mesh* CreateQuadMesh(const Vector2D& textureSize, const Vector2D& extents);

//------------------------------------------------------------------------------
