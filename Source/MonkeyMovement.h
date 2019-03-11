//------------------------------------------------------------------------------
//
// File Name:	MonkeyMovement.h
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

#include "Vector2D.h" // Vector2D


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
struct MapCollision;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{

	class MonkeyMovement : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		MonkeyMovement();

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Loads object data from a file.
		// Params:
		//   parser = The parser for the file.
		virtual void Deserialize(Parser& parser) override;

		// Saves object data to a file.
		// Params:
		//   parser = The parser for the file.
		virtual void Serialize(Parser& parser) const override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Map collision handler for Monkey objects.
		// Params:
		//   object = The monkey object.
		//   collision = Which sides the monkey collided on.
		friend void MonkeyMapCollisionHandler(GameObject& object, 
			const MapCollision& collision);

		// Collision handler for monkey.
		// Params:
		//   object = The monkey.
		//   other  = The object the monkey is colliding with.
		friend void MonkeyCollisionHandler(GameObject& object, GameObject& other);

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Moves horizontally based on input
		void MoveHorizontal(float dt) const;

		// Moves vertically based on input
		void MoveVertical(float dt);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Movement properties
		float monkeyWalkSpeed;
		float monkeyJumpSpeed;
		Vector2D gravity;
		float terminalVelocity;
		float gracePeriod;

		// Components
		Transform* transform;
		Physics* physics;

		// Misc
		bool onGround;
		bool hasJumped;
		float airTime;
		float movementLerp;

		friend class MonkeyAnimation;
	};
}

//------------------------------------------------------------------------------
