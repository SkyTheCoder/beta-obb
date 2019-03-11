//------------------------------------------------------------------------------
//
// File Name:	SpriteTilemap.h
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

#include "Sprite.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Tilemap;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class SpriteTilemap : public Sprite
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new sprite for use with a tilemap.
	SpriteTilemap();

	// Clone the sprite, returning a dynamically allocated copy.
	Component* Clone() const override;

	// Draw a sprite (Sprite can be textured or untextured).
	void Draw() override;

	// Sets the tilemap data that will be used by the sprite.
	// Params:
	//   map = A pointer to the tilemap resource.
	void SetTilemap(const Tilemap* map);

	// Converts a coordinate in tile space to world space.
	// Params:
	//   tilePos = The coordinate in tile space.
	// Returns:
	//   The coordinate in world space.
	Vector2D TileToWorld(Vector2D tilePos);

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The tilemap
	const Tilemap* map;
};

//------------------------------------------------------------------------------
