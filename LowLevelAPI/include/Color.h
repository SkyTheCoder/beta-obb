//------------------------------------------------------------------------------
//
// File Name:	Color.h
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

#include <iostream>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Pubic Structures:
//------------------------------------------------------------------------------

class Color
{
public:
	//------------------------------------------------------------------------------
	// Pubic Functions:
	//------------------------------------------------------------------------------

	// Constructors
	Color(); // Defaults to white
	Color(float r, float g, float b, float a = 1.0f);

	// Operators
	Color operator*(const Color& other) const; // Multiplies two colors component-wise
	Color operator+(const Color& other) const; // Adds two colors together
	Color operator-(const Color& other) const; // Subtracts two colors
	Color operator*(float scalar) const; // Scales a color by the given value.

	// Other functions
	void Clamp(); // Adjusts RGBA values so they lie between 0 and 1.

	//------------------------------------------------------------------------------
	// Pubic Variables:
	//------------------------------------------------------------------------------

	float r;
	float g;
	float b;
	float a;
};

namespace Colors
{
	extern const Color White;
	extern const Color Black;
	extern const Color Grey;

	extern const Color Red;
	extern const Color Green;
	extern const Color Blue;
	
	extern const Color Yellow;
	extern const Color Violet;
	extern const Color Aqua;

	extern const Color Orange;
	extern const Color LightBlue;
};

//------------------------------------------------------------------------------
// Pubic Functions:
//------------------------------------------------------------------------------

// Scales a color by the given value.
Color operator*(float scalar, const Color& color);

// Output a color to a stream
std::ostream& operator<<(std::ostream& os, const Color& color);

// Read a color from a stream
std::istream& operator>>(std::istream& is, Color& color);

//------------------------------------------------------------------------------
