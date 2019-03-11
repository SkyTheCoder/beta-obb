//------------------------------------------------------------------------------
//
// File Name:	Camera.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

struct Matrix3D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Camera
{
public:
	Camera();
	~Camera();

	// Translation - use this to pan
	const Vector2D& GetTranslation() const;
	void SetTranslation(const Vector2D& translation);

	// FOV - use this to zoom
	float GetFOV() const;
	void SetFOV(float angle);

	// Sets camera properties to default values
	// (Translation of [0, 0], distance of 1, no rotation.)
	void Reset();

	// Returns the view matrix constructed using this camera's attributes.
	const Matrix3D& GetViewMatrix() const;

	// Returns the view-projection matrix constructed using this camera's attributes.
	// Params:
	//   aspectRatio = The ratio of the viewport's width to its height.
	const Matrix3D& GetProjectionMatrix(float aspectRatio) const;

private:
	// Deleted to prevent issues with pimpl.
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	// Private implementation
	class Implementation;
	Implementation* pimpl;
};

//------------------------------------------------------------------------------
