//------------------------------------------------------------------------------
//
// File Name:	Transform.cpp
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

#include "Transform.h"

// Systems
#include "../include/Parser.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in Transform.h.
// You are free to change the contents of this structure as long as you do not
//   change the public method declarations declared in the header.

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Transform constructor.
// Params:
//	 x = Initial world position on the x-axis.
//	 y = Initial world position on the y-axis.
Transform::Transform(float x, float y) : Component("Transform"),
	translation(Vector2D(x, y)), scale(Vector2D(50.0f, 50.0f)), rotation(0.0f),
	matrix(CS230::Matrix2D()), isDirty(true)
{
}

// Transform constructor.
// Params:
//	 translation = World position of the object.
//   scale		 = Width and height of the object.
//   rotation	 = Rotation of the object about the z-axis.
Transform::Transform(Vector2D translation, Vector2D scale, float rotation) : Component("Transform"),
	translation(translation), scale(scale), rotation(rotation),
	matrix(CS230::Matrix2D()), isDirty(true)
{
}

// Clone the transform, returning a dynamically allocated copy.
Component* Transform::Clone() const
{
	return new Transform(*this);
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file.
void Transform::Deserialize(Parser& parser)
{
	parser.ReadVariable("translation", translation);
	parser.ReadVariable("rotation", rotation);
	parser.ReadVariable("scale", scale);
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file.
void Transform::Serialize(Parser& parser) const
{
	parser.WriteVariable("translation", translation);
	parser.WriteVariable("rotation", rotation);
	parser.WriteVariable("scale", scale);
}

// Get the transform matrix, based upon translation, rotation and scale settings.
// Returns:
//	 A reference to the component's matrix structure
const CS230::Matrix2D& Transform::GetMatrix()
{
	CalculateMatrices();

	return matrix;
}

// Get the inverse of the transform matrix.
// Returns:
//	 A reference to the component's inverse matrix.
const CS230::Matrix2D& Transform::GetInverseMatrix()
{
	CalculateMatrices();

	return inverseMatrix;
}

// Set the translation of a transform component.
// Params:
//	 translation = Reference to a translation vector.
void Transform::SetTranslation(const Vector2D& translation_)
{
	// If the given translation is different, store it and mark the current matrix as dirty.
	if (!AlmostEqual(translation, translation_))
	{
		translation = translation_;
		isDirty = true;
	}
}

// Get the translation of a transform component.
// Returns:
//	 A reference to the component's translation structure.
const Vector2D& Transform::GetTranslation() const
{
	return translation;
}

// Set the rotation of a transform component.
// Params:
//	 rotation = The rotation value (in radians).
void Transform::SetRotation(float rotation_)
{
	// If the given rotation is different, store it and mark the current matrix as dirty.
	if (rotation != rotation_)
	{
		rotation = rotation_;
		isDirty = true;
	}
}

// Get the rotation value of a transform component.
// Returns:
//	 The component's rotation value (in radians).
float Transform::GetRotation() const
{
	return rotation;
}

// Set the scale of a transform component.
// Params:
//	 translation = Reference to a scale vector.
void Transform::SetScale(const Vector2D& scale_)
{
	// If the given scale is different, store it and mark the current matrix as dirty.
	if (!AlmostEqual(scale, scale_))
	{
		scale = scale_;
		isDirty = true;
	}
}

// Get the scale of a transform component.
// Returns:
//	 A reference to the component's scale structure.
const Vector2D& Transform::GetScale() const
{
	return scale;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Calculates the transform matrix and its inverse using 
// translation, rotation, and scale.
void Transform::CalculateMatrices()
{
	// Check if the matrix needs to be recalculated.
	if (isDirty)
	{
		// Recalculate the matrix.
		CS230::Matrix2D translationMatrix = CS230::Matrix2D::TranslationMatrix(translation.x, translation.y);
		CS230::Matrix2D rotationMatrix = CS230::Matrix2D::RotationMatrixRadians(rotation);
		CS230::Matrix2D scalingMatrix = CS230::Matrix2D::ScalingMatrix(scale.x, scale.y);

		matrix = translationMatrix * rotationMatrix * scalingMatrix;

		// Recalculate the inverse matrix.
		CS230::Matrix2D invTranslationMatrix = CS230::Matrix2D::TranslationMatrix(-translation.x, -translation.y);
		CS230::Matrix2D invRotationMatrix = CS230::Matrix2D::RotationMatrixRadians(-rotation);
		CS230::Matrix2D invScalingMatrix = CS230::Matrix2D::ScalingMatrix(1.0f / scale.x, 1.0f / scale.y);

		inverseMatrix = invScalingMatrix * invRotationMatrix * invTranslationMatrix;

		// Mark the current matrix as clean.
		isDirty = false;
	}
}

//------------------------------------------------------------------------------
