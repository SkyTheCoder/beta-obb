//------------------------------------------------------------------------------
//
// File Name:	MonkeyAnimation.cpp
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

#include "MonkeyAnimation.h"

// Systems
#include "GameObject.h"
#include <Parser.h>

// Components
#include "Animation.h"
#include "Physics.h"
#include "Transform.h"
#include "MonkeyMovement.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//	 walkStart  = The starting frame for the walk animation.
	//   walkLength = The number of frames of the walk animation.
	//   jumpStart  = The starting frame for the jump animation.
	//   idleStart  = The starting frame for the idle animation.
	MonkeyAnimation::MonkeyAnimation(unsigned walkStart, unsigned walkLength,
		unsigned jumpStart, unsigned idleStart) : Component("MonkeyAnimation"),
		walkStart(walkStart), walkLength(walkLength), jumpStart(jumpStart), idleStart(idleStart),
		currentState(StateIdle), nextState(StateIdle)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* MonkeyAnimation::Clone() const
	{
		return new MonkeyAnimation(*this);
	}

	// Initialize this component (happens at object creation).
	void MonkeyAnimation::Initialize()
	{
		// Store the required components for ease of access.
		animation = GetOwner()->GetComponent<Animation>();
		physics = GetOwner()->GetComponent<Physics>();
		transform = GetOwner()->GetComponent<Transform>();
		monkeyMovement = GetOwner()->GetComponent<MonkeyMovement>();

		// Play the idle animation.
		animation->Play(idleStart, 1, 0.0f, true);

		// Backup the original scale.
		originalScale = transform->GetScale();
	}

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	void MonkeyAnimation::Deserialize(Parser& parser)
	{
		parser.ReadVariable("walkStart", walkStart);
		parser.ReadVariable("walkLength", walkLength);
		parser.ReadVariable("jumpStart", jumpStart);
		parser.ReadVariable("idleStart", idleStart);
	}

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	void MonkeyAnimation::Serialize(Parser& parser) const
	{
		parser.WriteVariable("walkStart", walkStart);
		parser.WriteVariable("walkLength", walkLength);
		parser.WriteVariable("jumpStart", jumpStart);
		parser.WriteVariable("idleStart", idleStart);
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void MonkeyAnimation::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Choose the next state.
		ChooseNextState();

		// Update the current state if necessary.
		ChangeCurrentState();

		// Flip the sprite if necessary.
		FlipSprite();
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Choose the correct state based on velocity.
	void MonkeyAnimation::ChooseNextState()
	{
		Vector2D velocity = physics->GetVelocity();
		// If we are jumping/falling, use the jumping state.
		if (monkeyMovement->airTime > 0.05f)
		{
			nextState = State::StateJump;
		}
		// If we are moving to the side, use the walking state.
		else if (abs(velocity.x) > 16.8f)
		{
			nextState = State::StateWalk;
		}
		// If we are standing still, use the idle state.
		else
		{
			nextState = State::StateIdle;
		}
	}

	// Change states and start the appropriate animation.
	void MonkeyAnimation::ChangeCurrentState()
	{
		// Check if the state should change.
		if (currentState != nextState)
		{
			currentState = nextState;
			switch (currentState)
			{
			// If the state is changed to the idle state, begin playing the idle animation.
			case State::StateIdle:
				animation->Play(idleStart, 1, 0.0f, true);
				break;
			// If the state is changed to the walking state, begin playing the walking animation.
			case State::StateWalk:
				animation->Play(walkStart, walkLength, 0.2f, true);
				break;
			// If the state is changed to the jumping state, begin playing the jumping animation.
			case State::StateJump:
				animation->Play(jumpStart, 1, 0.0f, true);
				break;
			}
		}
	}

	// Flip the sprite based on velocity and current state.
	void MonkeyAnimation::FlipSprite() const
	{
		// Check if we are walking or jumping.
		if (currentState != State::StateIdle)
		{
			Vector2D velocity = physics->GetVelocity();
			// If we are moving left, flip the sprite on the X axis.
			if (velocity.x < 0.0f)
			{
				transform->SetScale(Vector2D(-originalScale.x, originalScale.y));
			}
			// If we are moving right, restore the original scale.
			else if (velocity.x > 0.0f)
			{
				transform->SetScale(originalScale);
			}
		}
	}
}

//------------------------------------------------------------------------------
