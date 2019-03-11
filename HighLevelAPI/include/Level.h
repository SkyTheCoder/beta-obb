//------------------------------------------------------------------------------
//
// File Name:	Level.h
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

#include <BetaObject.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Space Space;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface (functions) declared in the header.
class Level : public BetaObject
{
public:
	// Creates a game state.
	// Params:
	//   name = Name to use for this state.
	Level(const std::string& name);

	// Retrieve the space in which the level is running.
	Space* GetSpace() const;
};

//------------------------------------------------------------------------------
