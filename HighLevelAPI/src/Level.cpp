//------------------------------------------------------------------------------
//
// File Name:	Level.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Level.h"

// Systems
#include "Space.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Creates a game state.
// Params:
//   name = Name to use for this state.
Level::Level(const std::string& name) : BetaObject(name)
{
}

// Retrieve the space in which the level is running.
Space* Level::GetSpace() const
{
	// The object's parent should be a space.
	return static_cast<Space*>(BetaObject::GetParent());
}

//------------------------------------------------------------------------------
