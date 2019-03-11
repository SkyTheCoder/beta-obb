//------------------------------------------------------------------------------
//
// File Name:	SpriteTilemap.cpp
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

#include "SpriteTilemap.h"

// Systems
#include "Tilemap.h"
#include "Transform.h"
#include <Graphics.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new sprite for use with a tilemap.
SpriteTilemap::SpriteTilemap() : map(nullptr)
{
}

// Clone the sprite, returning a dynamically allocated copy.
Component* SpriteTilemap::Clone() const
{
	return new SpriteTilemap(*this);
}

// Draw a sprite (Sprite can be textured or untextured).
void SpriteTilemap::Draw()
{
	// If there is no map, there is nothing to draw.
	if (map == nullptr)
		return;

	// Store variables that are frequently used in the loop.
	unsigned width = map->GetWidth();
	unsigned height = map->GetHeight();
	Vector2D translation = transform->GetTranslation();
	Vector2D scale = transform->GetScale();

	for (unsigned x = 0; x < width; x++)
	{
		for (unsigned y = 0; y < height; y++)
		{
			// Get the cell value at the current position.
			int cellValue = map->GetCellValue(x, y);

			// If the cell is empty, don't draw anything.
			if (cellValue < 1)
				continue;

			// Set the frame in the spritesheet.
			SetFrame(cellValue - 1);

			// Draw the tile.
			Vector2D offset = Vector2D(x * scale.x, y * -scale.y);

			const BoundingRectangle screenWorldDimensions = Graphics::GetInstance().GetScreenWorldDimensions();

			// If the tile would be off the screen, don't draw anything.
			if (screenWorldDimensions.bottom > translation.y + offset.y + scale.y * 2.0f
				|| screenWorldDimensions.top < translation.y + offset.y - scale.y * 2.0f
				|| screenWorldDimensions.left > translation.x + offset.x + scale.x * 2.0f
				|| screenWorldDimensions.right < translation.x + offset.x - scale.x * 2.0f)
				continue;

			Sprite::Draw(offset);
		}
	}
}

// Sets the tilemap data that will be used by the sprite.
// Params:
//   map = A pointer to the tilemap resource.
void SpriteTilemap::SetTilemap(const Tilemap* map_)
{
	map = map_;
}

// Converts a coordinate in tile space to world space.
// Params:
//   tilePos = The coordinate in tile space.
// Returns:
//   The coordinate in world space.
Vector2D SpriteTilemap::TileToWorld(Vector2D tilePos)
{
	return transform->GetMatrix() * Vector2D(tilePos.x, -tilePos.y);
}

//------------------------------------------------------------------------------
