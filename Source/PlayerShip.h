//------------------------------------------------------------------------------
//
// File Name:	PlayerShip.h
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{

	class PlayerShip : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   forwardThrust	= Amount of force added to the ship when thrusters are activated.
		//   maximumSpeed	= Maximum attainable speed of the ship.
		//   rotationSpeed	= Speed at which the ship rotates.
		//   bulletSpeed	= Speed at which bullets move when fired by ship.
		//   deathDuration	= Length of death "animation" in seconds.
		PlayerShip(float forwardThrust = 150.0f, float maximumSpeed = 100.0f, 
			float rotationSpeed = (float)M_PI, float bulletSpeed = 200.0f, float deathDuration = 4.0f);

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;
		
		// Returns player's score
		unsigned GetScore() const;

		// Increase player's score
		// Params:
		//   amount = This value is added to the player's current score.
		void IncreaseScore(unsigned amount);

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Move forward when up arrow is pressed
		void Move() const;

		// Rotate when left or right arrow key is pressed
		void Rotate() const;

		// Shoot projectiles when space is pressed
		void Shoot();

		// Play death "animation"
		// Params:
		//   dt = How much time has passed since the last frame.
		void DeathSequence(float dt);

		// Collision start event handler.
		// Params:
		//	 ship = The player ship.
		//   otherObject = The other object.
		static friend void CollisionHandlerShip(GameObject& ship, GameObject& otherObject);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Properties (save to/load from file)
		float forwardThrust;
		float maximumSpeed;
		float rotationSpeed;
		float bulletSpeed;
		float deathDuration;

		// Bullet archetype
		GameObject* bulletArchetype;

		// Components
		Transform* transform;
		Physics* physics;

		// Other variables
		float timer;
		bool isDying;
		unsigned score;
	};
}

//------------------------------------------------------------------------------
