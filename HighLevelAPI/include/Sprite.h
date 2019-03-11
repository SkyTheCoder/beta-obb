//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
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

#include "Component.h"
#include <Color.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Mesh;
class SpriteSource;
class Transform;
class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class Sprite : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new sprite object.
	Sprite();

	// Clone the sprite, returning a dynamically allocated copy.
	Component* Clone() const override;

	// Initialize components.
	void Initialize();

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	virtual void Deserialize(Parser& parser) override;

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	virtual void Serialize(Parser& parser) const override;

	// Draw a sprite (Sprite can be textured or untextured).
	void Draw() override;

	// Draw a sprite at an offset from the object's translation.
	// Params:
	//   offset = The offset that will be added to the translation when drawing.
	void Draw(const Vector2D& offset);

	// Set a sprite's transparency (between 0.0f and 1.0f).
	// Params:
	//   alpha = New value for the sprite's 'alpha' value.
	void SetAlpha(float alpha);
	
	// Get the current value for a sprite's transparency.
	float GetAlpha() const;

	// Set the sprite's current frame.
	// (NOTE: You must validate the frame index against the sprite source's frame count.)
	// Params:
	//   frameIndex = New frame index for the sprite (0 .. frame count).
	void SetFrame(unsigned int frameIndex);

	// Returns the index of the current frame being displayed.
	unsigned GetFrame() const;

	// Set the sprite's mesh.
	// (NOTE: This mesh may be textured or untextured.)
	// (NOTE: This mesh may contain any number of triangles.)
	// Params:
	//   mesh = Pointer to a mesh created using the Alpha Engine.
	void SetMesh(Mesh* mesh);

	// Set a new sprite source for the specified sprite.
	// Params:
	//	 spriteSource = A new sprite source for the sprite.
	void SetSpriteSource(SpriteSource* spriteSource);

	// Set the blend color for the specified sprite.
	// Params:
	//	 color = A new color for the sprite.
	void SetColor(Color color);

	// Retrieves the blend color for the sprite.
	const Color& GetColor() const;

protected:
	//------------------------------------------------------------------------------
	// Protected Variables:
	//------------------------------------------------------------------------------

	// Components
	Transform* transform;

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The sprite source used when drawing (simple colored mesh will have no sprite source).
	SpriteSource* spriteSource;

	// The mesh used to draw the sprite.
	Mesh* mesh;

	// Color used for blending/tint
	Color color;
};

//------------------------------------------------------------------------------
