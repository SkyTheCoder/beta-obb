//------------------------------------------------------------------------------
//
// File Name:	Matrix3D.h
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

#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Matrix2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Frequently, vector and matrix structures are declared publicly.  While an
// interface is typically provided with these structures, these are two common
// exceptions to the coding best practice of "hiding information".

// WARNING: Do not modify the declaration of this structure, as it is a part
// of the public interface.

struct Matrix3D
{
public:
	Matrix3D();
	Matrix3D(const Matrix2D& m);
	~Matrix3D();

	void* data;
	
private:
	// Deleted to prevent issues with pimpl.
	Matrix3D(const Matrix3D& rhs) = delete;
	Matrix3D& operator=(const Matrix3D& rhs) = delete;
};

//------------------------------------------------------------------------------
