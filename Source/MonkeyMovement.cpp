//------------------------------------------------------------------------------
//
// File Name:	MonkeyMovement.cpp
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

#include "MonkeyMovement.h"

// Systems
#include "GameObject.h"
#include <Input.h>
#include "Space.h"
#include <Interpolation.h>
#include <Parser.h>

// Components
#include "Transform.h"
#include "Physics.h"
#include "Collider.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Map collision handler for Monkey objects.
	// Params:
	//   object = The monkey object.
	//   collision = Which sides the monkey collided on.
	void MonkeyMapCollisionHandler(GameObject& object, const MapCollision& collision)
	{
		// Get the MonkeyMovement component.
		MonkeyMovement* monkeyMovement = object.GetComponent<MonkeyMovement>();

		// If the monkey's collider is colliding on the bottom, mark the monkey as on ground.
		if (collision.bottom)
		{
			monkeyMovement->onGround = true;
		}
	}

	// Collision handler for monkey.
	// Params:
	//   object = The monkey.
	//   other  = The object the monkey is colliding with.
	void MonkeyCollisionHandler(GameObject& object, GameObject& other)
	{
		UNREFERENCED_PARAMETER(object);

		// Destroy collectibles when touching.
		if (other.GetName() == "Collectible")
		{
			other.Destroy();
		}

		// Restart the level when touching hazards or enemies.
		if (other.GetName() == "Hazard")
		{
			object.GetSpace()->RestartLevel();
		}

		if (other.GetName() == "Enemy")
		{
			object.GetSpace()->RestartLevel();
		}
	}

	// Constructor
	MonkeyMovement::MonkeyMovement() : Component("MonkeyMovement"), monkeyWalkSpeed(300.0f), monkeyJumpSpeed(800.0f),
		gravity(0.0f, -1200.0f), terminalVelocity(700.0f), gracePeriod(0.15f),
		transform(nullptr), physics(nullptr),
		onGround(false), hasJumped(false), airTime(0.0f), movementLerp(0.998f)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* MonkeyMovement::Clone() const
	{
		return new MonkeyMovement(*this);
	}

	// Initialize this component (happens at object creation).
	void MonkeyMovement::Initialize()
	{
		// Store the required components for ease of access.
		transform = GetOwner()->GetComponent<Transform>();
		physics =GetOwner()->GetComponent<Physics>();

		Collider* collider = GetOwner()->GetComponent<Collider>();

		// Set the map collision handler.
		collider->SetMapCollisionHandler(MonkeyMapCollisionHandler);

		// Set the collision handler.
		collider->SetCollisionHandler(MonkeyCollisionHandler);
	}

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	void MonkeyMovement::Deserialize(Parser& parser)
	{
		parser.ReadVariable("monkeyWalkSpeed", monkeyWalkSpeed);
		parser.ReadVariable("monkeyJumpSpeed", monkeyJumpSpeed);
		parser.ReadVariable("gravity", gravity);
	}

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	void MonkeyMovement::Serialize(Parser& parser) const
	{
		parser.WriteVariable("monkeyWalkSpeed", monkeyWalkSpeed);
		parser.WriteVariable("monkeyJumpSpeed", monkeyJumpSpeed);
		parser.WriteVariable("gravity", gravity);
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void MonkeyMovement::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Handle horizontal movement.
		MoveHorizontal(dt);

		// Handle vertical movement.
		MoveVertical(dt);
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Moves horizontally based on input
	void MonkeyMovement::MoveHorizontal(float dt) const
	{
		Input& input = Input::GetInstance();

		Vector2D velocity = physics->GetVelocity();

		// Initialize target velocity to 0.
		float targetVelocityX = 0.0f;

		// If the right arrow key is pressed, move to the right.
		if (input.CheckHeld(VK_RIGHT))
		{
			targetVelocityX += monkeyWalkSpeed;
		}

		// If the right arrow key is pressed, move to the left.
		if (input.CheckHeld(VK_LEFT))
		{
			targetVelocityX -= monkeyWalkSpeed;
		}

		// Smoothly interpolate the X component of the player's velocity.
		float movementMix = 1.0f - pow(1.0f - movementLerp, dt);
		velocity.x = Interpolate(velocity.x, targetVelocityX, movementMix);

		// Set the velocity.
		physics->SetVelocity(velocity);
	}

	// Moves vertically based on input
	void MonkeyMovement::MoveVertical(float dt)
	{
		Input& input = Input::GetInstance();

		Vector2D velocity = physics->GetVelocity();

		if (onGround)
		{
			// Reset time spent in the air.
			airTime = 0.0f;

			// The player has not jumped since leaving the ground.
			hasJumped = false;
		}
		else
		{
			// Keep track of time spent in the air.
			airTime += dt;
		}

		// If the monkey has not jumped since landing, was on the ground recently, and the up arrow key is pressed, jump.
		if (!hasJumped && airTime <= gracePeriod && input.CheckHeld(VK_UP))
		{
			// Increase Y velocity.
			velocity.y = monkeyJumpSpeed;

			hasJumped = true;
		}
		
		// Apply gravity if in air.
		if (!onGround)
		{
			physics->AddForce(gravity);
		}

		// Clamp velocity.
		velocity.y = std::max(-terminalVelocity, std::min(terminalVelocity, velocity.y));

		// Set the velocity.
		physics->SetVelocity(velocity);

		onGround = false;
	}
}

//------------------------------------------------------------------------------
