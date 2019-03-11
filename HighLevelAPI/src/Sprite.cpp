//------------------------------------------------------------------------------
//
// File Name:	Sprite.cpp
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

#include "Sprite.h"

#include <Color.h>
#include <Vector2D.h>

// Systems
#include <Mesh.h>
#include "SpriteSource.h"
#include <Graphics.h>
#include "GameObject.h"
#include "Matrix2DStudent.h"
#include "Parser.h"
#include "Space.h"

// Components
#include "Transform.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new sprite object.
Sprite::Sprite() : Component("Sprite"), frameIndex(0), spriteSource(nullptr), mesh(nullptr), color(Colors::White), transform(nullptr)
{

}

// Clone the sprite, returning a dynamically allocated copy.
Component* Sprite::Clone() const
{
	return new Sprite(*this);
}

// Initialize components.
void Sprite::Initialize()
{
	transform = static_cast<Transform*>(GetOwner()->GetComponent<Transform>());
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file.
void Sprite::Deserialize(Parser& parser)
{
	parser.ReadVariable("frameIndex", frameIndex);
	parser.ReadVariable("color", color);
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file.
void Sprite::Serialize(Parser& parser) const
{
	parser.WriteVariable("frameIndex", frameIndex);
	parser.WriteVariable("color", color);
}

// Draw a sprite (Sprite can be textured or untextured).
void Sprite::Draw()
{
	Draw(Vector2D());
}

// Draw a sprite at an offset from the object's translation.
// Params:
//   offset = The offset that will be added to the translation when drawing.
void Sprite::Draw(const Vector2D& offset)
{
	// Exit if there is no transform.
	if (transform == nullptr)
	{
		return;
	}

	// Exit if there is no mesh.
	if (mesh == nullptr)
	{
		return;
	}

	// If there is a sprite source, get its UV and set the current texture. Otherwise, clear the current texture.
	if (spriteSource != nullptr)
	{
		Vector2D uv;
		spriteSource->GetUV(frameIndex, uv);
		Graphics::GetInstance().SetTexture(spriteSource->GetTexture(), uv);
	}
	else
	{
		Graphics::GetInstance().SetTexture(nullptr);
	}

	CS230::Matrix2D offsetTransform = CS230::Matrix2D::TranslationMatrix(offset.x, offset.y);
	CS230::Matrix2D fullTransform = offsetTransform * transform->GetMatrix();

	// Set the translation & scale for the mesh.
	Graphics::GetInstance().SetTransform(reinterpret_cast<const Matrix2D&>(fullTransform));

	Graphics::GetInstance().SetSpriteBlendColor(color);

	// Draw the mesh.
	mesh->Draw();
}

// Set a sprite's transparency (between 0.0f and 1.0f).
// Params:
//   alpha = New value for the sprite's 'alpha' value.
void Sprite::SetAlpha(float alpha)
{
	color.a = alpha;
}

// Get the current value for a sprite's transparency.
float Sprite::GetAlpha() const
{
	return color.a;
}

// Set the sprite's current frame.
// (NOTE: You must validate the frame index against the sprite source's frame count.)
// Params:
//   frameIndex = New frame index for the sprite (0 .. frame count).
void Sprite::SetFrame(unsigned int frameIndex_)
{
	frameIndex = frameIndex_;
}

// Returns the index of the current frame being displayed.
unsigned Sprite::GetFrame() const
{
	return frameIndex;
}

// Set the sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//   mesh = Pointer to a mesh created using the Alpha Engine.
void Sprite::SetMesh(Mesh* mesh_)
{
	mesh = mesh_;
}

// Set a new SpriteSource for the specified sprite.
// Params:
//	 spriteSource = A new sprite source for the sprite.
void Sprite::SetSpriteSource(SpriteSource* spriteSource_)
{
	spriteSource = spriteSource_;
}

// Set the blend color for the specified sprite.
// Params:
//	 color = A new color for the sprite.
void Sprite::SetColor(Color color_)
{
	color = color_;
}

// Retrieves the blend color for the sprite.
const Color& Sprite::GetColor() const
{
	return color;
}

//------------------------------------------------------------------------------
