//------------------------------------------------------------------------------
//
// File Name:	Physics.h
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

#include "Component.h"
#include <Vector2D.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Transform;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in Physics.c.
class Physics : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new physics component.
	Physics();

	// Clone the physics component, returning a dynamically allocated copy.
	Component* Clone() const override;

	// Initialize components.
	void Initialize() override;

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	virtual void Deserialize(Parser& parser) override;

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	virtual void Serialize(Parser& parser) const override;

	// Reset acceleration.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt) override;

	// Update velocity and translation.
	// Params:
	//	 dt = Change in time (in seconds) since the last fixed update.
	void FixedUpdate(float dt) override;

	// Set the velocity of a physics component.
	// Params:
	//	 velocity = Pointer to a velocity vector.
	void SetVelocity(const Vector2D& velocity);

	// Get the velocity of a physics component.
	// Returns:
	//		A reference to the component's velocity structure.
	const Vector2D& GetVelocity() const;

	// Set the angular velocity of a physics component.
	// Params:
	//	 velocity = New value for the rotational velocity.
	void SetAngularVelocity(float velocity);

	// Get the angular velocity of a physics component.
	// Returns:
	//	 A float representing the new angular velocity.
	float GetAngularVelocity() const;

	// Set the mass of the physics component.
	// Params:
	//   mass = The new mass of the physics component.
	void SetMass(float mass);

	// Add a force to the object.
	// Params:
	//	 force = A force vector with direction and magnitude.
	void AddForce(const Vector2D& force);

	// Get the acceleration of a physics component.
	// Returns:
	//	 A reference to the component's acceleration structure.
	const Vector2D& GetAcceleration() const;

	// Set the old translation (position) of a physics component.
	// Params: 
	//   oldTranslation = New value for the old translation.
	void SetOldTranslation(const Vector2D& oldTranslation);

	// Get the old translation (position) of a physics component.
	// Returns: 
	//   A reference to the component's oldTranslation structure.
	const Vector2D& GetOldTranslation() const;

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	velocity;

	// Angular velocity, the speed and direction that the object rotates.
	float		angularVelocity;

	// Used when calculating acceleration due to forces.
	float		inverseMass;

	// The sum of all forces acting on an object
	Vector2D	forcesSum;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	acceleration;

	// Previous position.  May be used for resolving collisions.
	Vector2D	oldTranslation;

	// Transform component
	Transform*	transform;
};

//------------------------------------------------------------------------------
