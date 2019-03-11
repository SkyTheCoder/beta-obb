//------------------------------------------------------------------------------
//
// File Name:	Component.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <BetaObject.h>

#include "Serializable.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class GameObject;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface (function prototypes) declared in the header.
class Component : public BetaObject, public Serializable
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor - needed to construct BetaObject.
	Component(const std::string& name);

	// Clones the component, returning a dynamically allocated copy.
	virtual Component* Clone() const = 0;

	// Retrieve component's owner object (if any).
	GameObject* GetOwner() const;
};

//------------------------------------------------------------------------------
