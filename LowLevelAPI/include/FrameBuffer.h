//------------------------------------------------------------------------------
//
// File Name:	FrameBuffer.h
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

//	An FBO is used by OpenGL to render into a texture. Specifically this implementation assumes that the
//	rendered mesh will provide diffuse, position and normal at the same time in a MRT fashion
//  http://www.codinglabs.net/tutorial_simple_def_rendering.aspx
class FrameBuffer
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Ctors/Dtors
	FrameBuffer(unsigned width, unsigned height);
	~FrameBuffer();

	// Methods
	void Start();
	void Stop();
	void ShowTexture(unsigned i, float fSizeX = 400, float fSizeY = 400, float x = 0, float y = 0) const;

	unsigned GetDiffuseTexture() const;
	unsigned GetPositionTexture() const;
	unsigned GetNormalsTexture() const;

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Variables
	unsigned fbo;			  // The FBO ID
	unsigned diffuseRT;		  // The diffuse render target
	unsigned diffuseTexture;  // The OpenGL texture for the diffuse render target
	unsigned positionRT;	  // The position render target
	unsigned positionTexture; // The OpenGL texture for the position render target
	unsigned normalsRT;		  // The normals render target
	unsigned normalsTexture;  // The OpenGL texture for the normals render target
	unsigned depthBuffer;	  // Depth buffer handle

	unsigned width;			  // FBO width
	unsigned height;		  // FBO height
};

//------------------------------------------------------------------------------
