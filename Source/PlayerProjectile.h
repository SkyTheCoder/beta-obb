//------------------------------------------------------------------------------
//
// File Name:	PlayerProjectile.h
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

#include "Component.h" // base class

#include "PlayerShip.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{

	class PlayerProjectile : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Default constructor
		PlayerProjectile();

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Set the object that spawned this object
		void SetSpawner(PlayerShip* player);

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// The collision handling function for Asteroids.
		// Params:
		//	 bullet = The bullet object.
		//   other = The other object.
		friend static void CollisionHandlerBullet(GameObject& bullet, GameObject& otherObject);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Who spawned this object?
		Behaviors::PlayerShip* player;
	};
}

//------------------------------------------------------------------------------
