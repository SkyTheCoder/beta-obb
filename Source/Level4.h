//------------------------------------------------------------------------------
//
// File Name:	Level4.h
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

#include <Level.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class Level4 : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Level 4.
		Level4();

		// Load the resources associated with Level 4.
		void Load() override;

		// Initialize the memory associated with Level 4.
		void Initialize() override;

		// Update Level 4.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with Level 4.
		void Unload() override;
	};
}

//----------------------------------------------------------------------------
