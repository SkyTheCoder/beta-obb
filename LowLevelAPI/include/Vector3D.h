//------------------------------------------------------------------------------
//
// File Name:	Vector3D.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaEngine
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
// Forward References:
//------------------------------------------------------------------------------

class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Frequently, vector and matrix structures are declared publicly.  While an
// interface is typically provided with these structures, these are two common
// exceptions to the coding best practice of "hiding information".

class Vector3D
{
public:
	//------------------------------------------------------------------------------
	// Public Functions
	//------------------------------------------------------------------------------

	// Constructors (3)
	Vector3D();
	Vector3D(const Vector2D& vec2);
	Vector3D(float x, float y, float z);

	// Other functions (7)
	float MagnitudeSquared() const;
	float Magnitude() const;
	Vector3D Normalized() const;
	float DistanceSquared(const Vector3D& other) const;
	float Distance(const Vector3D& other) const;
	Vector3D Midpoint(const Vector3D& other) const;
	float DotProduct(const Vector3D& other) const;
	Vector3D CrossProduct(const Vector3D& other) const;

	// Overloaded operators (7 member functions)
	Vector3D operator+(const Vector3D& other) const;
	Vector3D operator-(const Vector3D& other) const;
	Vector3D operator*(float scalar) const;
	Vector3D operator/(float scalar) const;
	Vector3D operator-() const;
	Vector3D& operator+=(const Vector3D& other);
	Vector3D& operator-=(const Vector3D& other);
	Vector3D& operator*=(float scalar);
	Vector3D& operator/=(float scalar);

	//------------------------------------------------------------------------------
	// Public Data
	//------------------------------------------------------------------------------

	float x; // The x-coordinate of a Vector3D
	float y; // The y-coordinate of a Vector3D
	float z; // The z-coordinate of a Vector3D
};

//------------------------------------------------------------------------------
// Public Functions (non-member, non-friend)
//------------------------------------------------------------------------------

// Overloaded operators (2 non-member, non-friend function)
Vector3D operator*(float scalar, const Vector3D& vector);

//------------------------------------------------------------------------------
