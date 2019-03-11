//------------------------------------------------------------------------------
//
// File Name:	Renderer.h
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Renderer
{
public:
	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	// Ctors/Dtors
	Renderer(unsigned width, unsigned height, const std::string& shaderFileName);
	virtual ~Renderer();

	// Methods
	virtual void Init();
	virtual void FrameStart() = 0;
	virtual void FrameEnd(bool syncOperations = false) = 0;

	unsigned GetShaderIndex() const;
	void SetShaderIndex(unsigned index);

	unsigned GetWidth() const;
	unsigned GetHeight() const;

	virtual void SetDimensions(unsigned width, unsigned height);

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	unsigned shaderIndex; // Index of shader in ShaderManager.

	unsigned width; // Render width
	unsigned height; // Render height
};

//------------------------------------------------------------------------------
