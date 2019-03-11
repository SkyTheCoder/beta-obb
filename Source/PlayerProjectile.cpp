//------------------------------------------------------------------------------
//
// File Name:	PlayerProjectile.cpp
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

#include "PlayerProjectile.h"

// Systems
#include <Engine.h>
#include <GameObject.h>

// Components
#include <Collider.h>
#include "PlayerShip.h"
#include "Asteroid.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	// The collision handling function for Asteroids.
	// Params:
	//	 bullet = The bullet object.
	//   other = The other object.
	void CollisionHandlerBullet(GameObject& bullet, GameObject& otherObject)
	{
		
		// Check if the projectile collided with an asteroid.
		if (otherObject.GetName() == "Asteroid")
		{
			// Increase the player's score by the asteroid's point value.
			bullet.GetComponent<PlayerProjectile>()->player->IncreaseScore(otherObject.GetComponent<Asteroid>()->GetPointValue());

			// Destroy the bullet.
			bullet.Destroy();
		}
	}

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Default constructor
	PlayerProjectile::PlayerProjectile() : Component("PlayerProjectile"), player(nullptr)
	{
	}

	// Initialize this component (happens at object creation).
	void PlayerProjectile::Initialize()
	{
		// Add the collision handler to the collider.
		GetOwner()->GetComponent<Collider>()->SetCollisionHandler(CollisionHandlerBullet);
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerProjectile::Clone() const
	{
		return new PlayerProjectile(*this);
	}

	// Set the object that spawned this object
	void PlayerProjectile::SetSpawner(PlayerShip* player_)
	{
		player = player_;
	}
}

//------------------------------------------------------------------------------
