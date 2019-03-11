//------------------------------------------------------------------------------
//
// File Name:	TimedDeath.cpp
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

#include "TimedDeath.h"

// Systems
#include "GameObject.h"
#include <Parser.h>

// Components
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Non-default constructor
	// Params:
	//   timeUntilDeath = Amount of time until the object self-destructs.
	TimedDeath::TimedDeath(float timeUntilDeath) : Component("TimedDeath"), timeUntilDeath(timeUntilDeath)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* TimedDeath::Clone() const
	{
		return new TimedDeath(*this);
	}

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	void TimedDeath::Deserialize(Parser& parser)
	{
		parser.ReadVariable("timeUntilDeath", timeUntilDeath);
	}

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	void TimedDeath::Serialize(Parser& parser) const
	{
		parser.WriteVariable("timeUntilDeath", timeUntilDeath);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void TimedDeath::Update(float dt)
	{
		timeUntilDeath -= dt;

		// Check if it's time to die (r/nocontext).
		if (timeUntilDeath <= 0.0f)
		{
			// IT'S HAPPENING, TIME'S UP!
			// Destroy the owner object.
			GetOwner()->Destroy();
		}
	}
}

//------------------------------------------------------------------------------
