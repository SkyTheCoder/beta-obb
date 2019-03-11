//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.cpp
// Author(s):	 David Cohen (david.cohen)
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

#include "SpriteSource.h"

#include <Texture.h>
#include <Vector2D.h>
#include "Sprite.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new sprite source object.
// Params:
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//	 texture = A pointer to a texture that has been loaded by the Alpha Engine.
SpriteSource::SpriteSource(int numCols, int numRows, Texture* texture) : numCols(numCols), numRows(numRows), texture(texture)
{
}

// Returns a pointer to the texture, for the purposes of rendering a sprite.
// Returns:
//	 A pointer to the sprite source texture.
Texture* SpriteSource::GetTexture() const
{
	return texture;
}

// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
// Returns:
//	 The calculated frame count.
unsigned SpriteSource::GetFrameCount() const
{
	return numCols * numRows;
}

// Stores the UV coordinates of the specified frame in the given vector.
// Params:
//	 frameIndex = A frame index within a sprite sheet.
//   textureCoords = Vector that will contain the UV coordinates.
void SpriteSource::GetUV(unsigned int frameIndex, Vector2D& textureCoords) const
{
	// Calculate the size of a single cell on the UV.
	float uSize = 1.0f / numCols;
	float vSize = 1.0f / numRows;

	// Calculate the column & row of the specified frame index.
	int col = frameIndex % numCols;
	int row = frameIndex / numCols;

	// Set the output texture coordinates to the proper location.
	textureCoords.x = col * uSize;
	textureCoords.y = row * vSize;
}

// Returns the number of rows in the sprite source.
unsigned SpriteSource::GetNumRows() const
{
	return numRows;
}

// Returns the number of columns in the sprite source.
unsigned SpriteSource::GetNumCols() const
{
	return numCols;
}

//----------------------------------------------------------------------------
