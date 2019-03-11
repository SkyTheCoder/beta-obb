//------------------------------------------------------------------------------
//
// File Name:	PlayerShip.cpp
// Author(s):	David Cohen (david.cohen)
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

#include "stdafx.h"

#include "PlayerShip.h"

// Systems
#include "GameObject.h"
#include "Space.h"
#include "GameObjectManager.h"
#include <Input.h>
#include <Parser.h>
#include <Engine.h>

// Components
#include "Transform.h"
#include "Physics.h"
#include <Collider.h>
#include "PlayerProjectile.h"
#include <Sprite.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	// Collision start event handler.
	// Params:
	//	 ship = The player ship.
	//   otherObject = The other object.
	void CollisionHandlerShip(GameObject& ship, GameObject& otherObject)
	{
		PlayerShip* playerShip = ship.GetComponent<PlayerShip>();

		// Check if the ship collided with an asteroid.
		if (!playerShip->isDying && otherObject.GetName() == "Asteroid")
		{
			// Begin the death animation.
			playerShip->isDying = true;
			playerShip->timer = playerShip->deathDuration;
		}
	}

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   forwardThrust = Amount of force added to the ship when thrusters are activated.
	//   maximumSpeed  = Maximum attainable speed of the ship.
	//   rotationSpeed = Speed at which the ship rotates.
	//   bulletSpeed   = Speed at which bullets move when fired by ship.
	PlayerShip::PlayerShip(float forwardThrust, float maximumSpeed, float rotationSpeed,
		float bulletSpeed, float deathDuration) : Component("PlayerShip"),
		forwardThrust(forwardThrust), maximumSpeed(maximumSpeed), rotationSpeed(rotationSpeed), bulletSpeed(bulletSpeed), deathDuration(deathDuration),
		timer(0.0f), isDying(false), score(0),
		bulletArchetype(nullptr), transform(nullptr), physics(nullptr)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerShip::Clone() const
	{
		return new PlayerShip(*this);
	}

	// Initialize this component (happens at object creation).
	void PlayerShip::Initialize()
	{
		// Store the required components for ease of access.
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();

		// Add the collision handler to the collider.
		GetOwner()->GetComponent<Collider>()->SetCollisionHandler(CollisionHandlerShip);

		GameObject* owner = GetOwner();
		Space* space = owner->GetSpace();
		GameObjectManager& objectManager = space->GetObjectManager();

		// Store the required archetypes for ease of access.
		bulletArchetype = objectManager.GetArchetypeByName("Bullet");
	}

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	void PlayerShip::Serialize(Parser& parser) const
	{
		parser.WriteVariable("forwardThrust", forwardThrust);
		parser.WriteVariable("maximumSpeed", maximumSpeed);
		parser.WriteVariable("rotationSpeed", rotationSpeed);
		parser.WriteVariable("bulletSpeed", bulletSpeed);
		parser.WriteVariable("deathDuration", deathDuration);
	}

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	void PlayerShip::Deserialize(Parser& parser)
	{
		parser.ReadVariable("forwardThrust", forwardThrust);
		parser.ReadVariable("maximumSpeed", maximumSpeed);
		parser.ReadVariable("rotationSpeed", rotationSpeed);
		parser.ReadVariable("bulletSpeed", bulletSpeed);
		parser.ReadVariable("deathDuration", deathDuration);
	}

	// Returns player's score
	unsigned PlayerShip::GetScore() const
	{
		return score;
	}

	// Increase player's score
	// Params:
	//   amount = This value is added to the player's current score.
	void PlayerShip::IncreaseScore(unsigned amount)
	{
		score += amount;
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerShip::Update(float dt)
	{
		if (!isDying)
		{
			// Handle movement (forward thrust).
			Move();

			// Handle rotation.
			Rotate();

			// Handle shooting.
			Shoot();
		}
		else
		{
			// Play the death animation.
			DeathSequence(dt);
		}
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Move forward when up arrow is pressed
	void PlayerShip::Move() const
	{
		Input& input = Input::GetInstance();

		// Check if the up arrow key is pressed.
		if (input.CheckHeld(VK_UP))
		{
			// Add force in the direction the ship is facing.
			physics->AddForce(Vector2D::FromAngleRadians(transform->GetRotation()) * forwardThrust);

			// Clamp velocity to the maximum speed.
			if (physics->GetVelocity().Magnitude() > maximumSpeed)
			{
				physics->SetVelocity(physics->GetVelocity().Normalized() * maximumSpeed);
			}
		}
	}

	// Rotate when left or right arrow key is pressed
	void PlayerShip::Rotate() const
	{
		Input& input = Input::GetInstance();

		float angularVelocity = 0.0f;

		// If the right arrow key is pressed, turn right.
		if (input.CheckHeld(VK_RIGHT))
		{
			angularVelocity -= rotationSpeed;
		}

		// If the left arrow key is pressed, turn left.
		if (input.CheckHeld(VK_LEFT))
		{
			angularVelocity += rotationSpeed;
		}
		
		// Set the angular velocity.
		physics->SetAngularVelocity(angularVelocity);
	}

	// Shoot projectiles when space is pressed
	void PlayerShip::Shoot()
	{
		Input& input = Input::GetInstance();

		// Check if the space key is pressed.
		if (input.CheckTriggered(VK_SPACE))
		{
			// Create a new instance of the bullet archetype.
			GameObject* bullet = new GameObject(*bulletArchetype);

			// Calculate the direction the ship the ship is facing.
			Vector2D direction = Vector2D::FromAngleRadians(transform->GetRotation());
			
			Transform* bulletTransform = bullet->GetComponent<Transform>();

			// Place the bullet in front of the ship.
			bulletTransform->SetTranslation(transform->GetTranslation() + direction * 32.0f);
			bulletTransform->SetRotation(transform->GetRotation());

			// Set the bullet's velocity.
			bullet->GetComponent<Physics>()->SetVelocity(direction * bulletSpeed);

			// Set the bullet's spawner.
			bullet->GetComponent<PlayerProjectile>()->SetSpawner(this);

			// Add the bullet to the object manager.
			GetOwner()->GetSpace()->GetObjectManager().AddObject(*bullet);
		}
	}

	// Play death "animation"
	// Params:
	//   dt = How much time has passed since the last frame.
	void PlayerShip::DeathSequence(float dt)
	{
		timer -= dt;

		// Restart the level when the animation completes.
		if (timer <= 0.0f)
		{
			GetOwner()->GetSpace()->RestartLevel();
			return;
		}

		// Make the ship spin out of control.
		physics->SetAngularVelocity(static_cast<float>(M_PI));

		Sprite* sprite = GetOwner()->GetComponent<Sprite>();

		// Make the ship flash red.
		sprite->SetColor(Color(1.0f, sin(timer * 4.0f * static_cast<float>(M_PI)) * 0.25f + 0.25f, 0.0f));

		// Make the ship fade in and out.
		sprite->SetAlpha(cos(timer * 4.0f * static_cast<float>(M_PI)) * 0.25f + 0.75f);
	}
}

//------------------------------------------------------------------------------
