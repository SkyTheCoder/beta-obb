//------------------------------------------------------------------------------
//
// File Name:	Physics.cpp
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

#include "Physics.h"

// Systems
#include "GameObject.h"
#include "Parser.h"

// Components
#include "Transform.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new physics component.
// Params:
//   transform - The transform of the object.
Physics::Physics() : Component("Physics"),
	velocity(Vector2D()), angularVelocity(0.0f), inverseMass(1.0f), forcesSum(Vector2D()),
	acceleration(Vector2D()), oldTranslation(Vector2D()), transform(nullptr)
{
}

// Clone the physics component, returning a dynamically allocated copy.
Component* Physics::Clone() const
{
	return new Physics(*this);
}

// Initialize components.
void Physics::Initialize()
{
	transform = static_cast<Transform*>(GetOwner()->GetComponent<Transform>());
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file.
void Physics::Deserialize(Parser& parser)
{
	parser.ReadVariable("acceleration", acceleration);
	parser.ReadVariable("velocity", velocity);
	parser.ReadVariable("angularVelocity", angularVelocity);
	float mass;
	parser.ReadVariable("mass", mass);
	inverseMass = 1.0f / mass;
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file.
void Physics::Serialize(Parser& parser) const
{
	parser.WriteVariable("acceleration", acceleration);
	parser.WriteVariable("velocity", velocity);
	parser.WriteVariable("angularVelocity", angularVelocity);
	float mass = 1.0f / inverseMass;
	parser.WriteVariable("mass", mass);
}

// Reset acceleration.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Physics::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	
	// Calculate the acceleration and clear the current forces.
	acceleration = forcesSum * inverseMass;
	forcesSum = Vector2D();
}

// Update velocity and translation.
// Params:
//	 dt = Change in time (in seconds) since the last fixed update.
void Physics::FixedUpdate(float dt)
{
	// Increase velocity by the acceleration from the last update.
	velocity += acceleration * dt;

	// Store the old translation.
	oldTranslation = transform->GetTranslation();

	// Add the current velocity to the translation & rotation.
	Vector2D newTranslation = oldTranslation + velocity * dt;
	float newRotation = transform->GetRotation() + angularVelocity * dt;
	
	// Update the translation & rotation.
	transform->SetTranslation(newTranslation);
	transform->SetRotation(newRotation);
}

// Set the velocity of a physics component.
// Params:
//	 velocity = Pointer to a velocity vector.
void Physics::SetVelocity(const Vector2D& velocity_)
{
	velocity = velocity_;
}

// Get the velocity of a physics component.
// Returns:
//		A reference to the component's velocity structure.
const Vector2D& Physics::GetVelocity() const
{
	return velocity;
}

// Set the angular velocity of a physics component.
// Params:
//	 velocity = New value for the rotational velocity.
void Physics::SetAngularVelocity(float velocity_)
{
	angularVelocity = velocity_;
}

// Get the angular velocity of a physics component.
// Returns:
//	 A float representing the new angular velocity.
float Physics::GetAngularVelocity() const
{
	return angularVelocity;
}

// Set the mass of the physics component.
// Params:
//   mass = The new mass of the physics component.
void Physics::SetMass(float mass)
{
	// We aren't using the mass for anything besides calculating acceleration, so we only need to store the inverse mass.
	inverseMass = 1.0f / mass;
}

// Add a force to the object.
// Params:
//	 force = A force vector with direction and magnitude.
void Physics::AddForce(const Vector2D& force)
{
	forcesSum += force;
}

// Get the acceleration of a physics component.
// Returns:
//	 A reference to the component's acceleration structure.
const Vector2D& Physics::GetAcceleration() const
{
	return acceleration;
}

// Set the old translation (position) of a physics component.
// Params: 
//   oldTranslation = New value for the old translation.
void Physics::SetOldTranslation(const Vector2D& oldTranslation_)
{
	oldTranslation = oldTranslation_;
}

// Get the old translation (position) of a physics component.
// Returns: 
//   A reference to the component's oldTranslation structure,
const Vector2D& Physics::GetOldTranslation() const
{
	return oldTranslation;
}

//------------------------------------------------------------------------------
