//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.cpp
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

#include "Matrix2DStudent.h"

//------------------------------------------------------------------------------

namespace CS230
{
	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	// Frequently, vector and matrix structures are declared publicly.  While an
	// interface is typically provided with these structures, these are two common
	// exceptions to the coding best practice of "hiding information".

	// WARNING: Do not modify the declaration of this structure, as it is a part
	// of the public interface.

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor for matrix. Sets all data in matrix to 0.
	Matrix2D::Matrix2D()
	{
		// Zero out the internal matrix.
		for (unsigned row = 0; row < 3; row++)
		{
			for (unsigned column = 0; column < 3; column++)
			{
				m[row][column] = 0.0f;
			}
		}
	}

	// This function creates an identity matrix and returns it.
	Matrix2D Matrix2D::IdentityMatrix()
	{
		Matrix2D matrix;

		for (unsigned diagonal = 0; diagonal < 3; diagonal++)
		{
			matrix.m[diagonal][diagonal] = 1.0f;
		}

		return matrix;
	}

	// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
	// (NOTE: Care must be taken when pResult = pMtx.)
	Matrix2D Matrix2D::Transposed() const
	{
		Matrix2D matrix;

		for (unsigned row = 0; row < 3; row++)
		{
			for (unsigned column = 0; column < 3; column++)
			{
				matrix.m[row][column] = m[column][row];
			}
		}

		return matrix;
	}

	// This function creates a translation matrix from x & y and returns it.
	Matrix2D Matrix2D::TranslationMatrix(float x, float y)
	{
		Matrix2D matrix = Matrix2D::IdentityMatrix();

		matrix.m[0][2] = x;
		matrix.m[1][2] = y;

		return matrix;
	}

	// This function creates a scaling matrix from x & y and returns it.
	Matrix2D Matrix2D::ScalingMatrix(float x, float y)
	{
		Matrix2D matrix = Matrix2D::IdentityMatrix();

		matrix.m[0][0] *= x;
		matrix.m[1][1] *= y;

		return matrix;
	}

	// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
	// Return the resulting matrix.
	// Converting from degrees to radians can be performed as follows:
	//	 radians = (angle * M_PI) / 180.0f
	Matrix2D Matrix2D::RotationMatrixDegrees(float angle)
	{
		Matrix2D matrix;

		// Convert angle from degrees to radians.
		angle = angle / 180.0f * static_cast<float>(M_PI);

		matrix.m[0][0] = cos(angle);
		matrix.m[0][1] = -sin(angle);
		matrix.m[1][0] = sin(angle);
		matrix.m[1][1] = cos(angle);

		matrix.m[2][2] = 1.0f;

		return matrix;
	}

	// This matrix creates a rotation matrix from "Angle" whose value is in radians.
	// Return the resulting matrix.
	Matrix2D Matrix2D::RotationMatrixRadians(float angle)
	{
		Matrix2D matrix;

		matrix.m[0][0] = cos(angle);
		matrix.m[0][1] = -sin(angle);
		matrix.m[1][0] = sin(angle);
		matrix.m[1][1] = cos(angle);

		matrix.m[2][2] = 1.0f;

		return matrix;
	}

	// This function multiplies the current matrix with another and returns the result as a new matrix.
	// HINT: Use the RowColumnMultiply function to help write this function.
	Matrix2D Matrix2D::operator*(const Matrix2D& other) const
	{
		Matrix2D matrix;

		for (unsigned row = 0; row < 3; row++)
		{
			for (unsigned col = 0; col < 3; col++)
			{
				matrix.m[row][col] = RowColumnMultiply(other, row, col);
			}
		}

		return matrix;
	}

	// This function multiplies the current matrix with another, storing the result in the current matrix.
	// HINT: Use the RowColumnMultiply function to help write this function.
	Matrix2D& Matrix2D::operator*=(const Matrix2D& other)
	{
		*this = *this * other;

		return *this;
	}

	// This function multiplies a matrix with a vector and returns the result as a new vector.
	// Result = Mtx * Vec.
	Vector2D Matrix2D::operator*(const Vector2D& vec) const
	{
		Vector2D output;

		// Assumes vec.z is 1 - when applying the matrix to a VECTOR instead of a POINT, vec.z should be 0!
		output.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2];
		output.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2];

		return output;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Multiplies a row of the current matrix by a column in another and returns the result.
	float Matrix2D::RowColumnMultiply(const Matrix2D& other, unsigned row, unsigned col) const
	{
		float sum = 0.0f;

		for (unsigned i = 0; i < 3; i++)
		{
			sum += m[row][i] * other.m[i][col];
		}

		return sum;
	}

}

//------------------------------------------------------------------------------
