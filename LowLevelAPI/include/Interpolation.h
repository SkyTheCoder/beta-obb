//------------------------------------------------------------------------------
//
// File Name:	Utility.h
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

#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Color;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

enum InterpolationMode
{
	IM_Linear,
	IM_Spherical,
	IM_LinearNormalized
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Linearly interpolates between two floating point values.
float Interpolate(float start, float end, float percent);

// Interpolates between two vectors using the specified interpolation method (defaults to linear interpolation).
Vector2D Interpolate(const Vector2D& start, const Vector2D& end, float percent, InterpolationMode mode = IM_Linear);

// Linearly interpolates between two colors.
Color Interpolate(const Color& start, const Color& end, float percent);

//------------------------------------------------------------------------------
