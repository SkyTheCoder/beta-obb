//------------------------------------------------------------------------------
//
// File Name:	Component.cpp
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

#include "Component.h"

// Systems
#include "GameObject.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor - needed to construct BetaObject.
Component::Component(const std::string& name) : BetaObject(name)
{
}

// Retrieve component's owner object (if any).
GameObject* Component::GetOwner() const
{
	return static_cast<GameObject*>(GetParent());
}

//------------------------------------------------------------------------------
